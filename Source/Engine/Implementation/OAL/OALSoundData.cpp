#include "OALSoundData.hpp"
#include "../../Core/Log.hpp"
#include "../../Core/BinaryFile.hpp"
#include "../../Core/Utils.hpp"
#include "OALUtils.hpp"
#include <stb_vorbis.c>
#include <vector>

COALSoundData::COALSoundData(const std::string& Name):
    ISoundData(Name)
{
}

COALSoundData::~COALSoundData()
{
    alDeleteBuffers( 1, &Buffer );
}

namespace
{
    bool LoadWav(const std::string& File, std::vector<char>& Data, ALenum& oFormat, ALsizei& Freq)
    {
        CBinaryFile Stream( File, EFileMode::Read );
        if( Stream )
        {
            // RIFF Header
            {
                char RiffID[4] = { '\0' };
                Stream.Read( RiffID, sizeof(char), 4 );
                if( std::string( RiffID, RiffID+4 ) != "RIFF" )
                {
                    LOG( ESeverity::Error ) << "Invalid WAV file: " << File << "\n";
                    return false;
                }

                int32_t ChunkSize = 0;
                Stream.Read( &ChunkSize, sizeof(int32_t), 1 );

                char Format[4] = { '\0' };
                Stream.Read( Format, sizeof(char), 4 );
                if( std::string( Format, Format+4 ) != "WAVE" )
                {
                    LOG( ESeverity::Error ) << "Invalid WAV file: " << File << "\n";
                    return false;
                }
            }
            // FMT Chunk
            {
                char Subchunk1ID[4] = { '\0' };
                Stream.Read( Subchunk1ID, sizeof(char), 4 );
                if( std::string( Subchunk1ID, Subchunk1ID+4 ) != "fmt " )
                {
                    LOG( ESeverity::Error ) << "Invalid WAV file: " << File << "\n";
                    return false;
                }

                int32_t Subchunk1Size = 0;
                Stream.Read( &Subchunk1Size, sizeof(int32_t), 1 );

                int16_t AudioFormat = 0;
                Stream.Read( &AudioFormat, sizeof(int16_t), 1 );
                if( AudioFormat != 1 )
                {
                    LOG( ESeverity::Error ) << "Invalid WAV Compression: " << File << "\n";
                    return false;
                }

                int16_t NumChannels = 0;
                Stream.Read( &NumChannels, sizeof(int16_t), 1 );

                int32_t SampleRate = 0;
                Stream.Read( &SampleRate, sizeof(int32_t), 1 );
                Freq = SampleRate;

                int32_t ByteRate = 0;
                Stream.Read( &ByteRate, sizeof(int32_t), 1 );

                int16_t BlockAlign = 0;
                Stream.Read( &BlockAlign, sizeof(int16_t), 1 );

                int16_t BitsPerSample = 0;
                Stream.Read( &BitsPerSample, sizeof(int16_t), 1 );

                switch( NumChannels )
                {
                case 1:
                    if( BitsPerSample == 8 ) oFormat = AL_FORMAT_MONO8;
                    else if( BitsPerSample == 16 ) oFormat = AL_FORMAT_MONO16;
                    else
                    {
                        LOG( ESeverity::Error ) << "Invalid WAV Format: " << File << "\n";
                        return false;
                    }
                    break;
                case 2:
                    if( BitsPerSample == 8 ) oFormat = AL_FORMAT_STEREO8;
                    else if( BitsPerSample == 16 ) oFormat = AL_FORMAT_STEREO16;
                    else
                    {
                        LOG( ESeverity::Error ) << "Invalid WAV Format: " << File << "\n";
                        return false;
                    }
                    break;
                default:
                    LOG( ESeverity::Error ) << "Invalid WAV Format: " << File << "\n";
                    return false;
                }

                if( Subchunk1Size > 16 )
                {
                    for(int i = Subchunk1Size; i > 16; --i)
                    {
                        char c = '\0';
                        Stream.Read( &c, sizeof(char), 1 );
                    }
                }
            }
            // DATA Chunk
            {
                char Subchunk2ID[4] = { '\0' };
                Stream.Read( Subchunk2ID, sizeof(char), 4 );
                if( std::string( Subchunk2ID, Subchunk2ID+4 ) != "data" )
                {
                    LOG( ESeverity::Error ) << "Invalid WAV file: " << File << "\n";
                    return false;
                }

                int32_t Subchunk2Size = 0;
                Stream.Read( &Subchunk2Size, sizeof(int32_t), 1 );

                Data.resize( Subchunk2Size );
                Stream.Read( Data.data(), sizeof(char), Subchunk2Size );
            }
            return true;
        }
        else
        {
            LOG( ESeverity::Error ) << "Unable Open File specified: " << File << "\n";
            return false;
        }
    }
}

bool COALSoundData::Load(CResources*, const ResourceCreateMap&)
{
    alGenBuffers( 1, &Buffer );
    if( OAL::CheckOALError() )
    {
        return false;
    }

    if( Utils::ToLower(Utils::GetFileExt( GetPath() )) == "wav" )
    {
        std::vector<char> WavData;
        ALenum Format;
        ALsizei Freq;
        if( !LoadWav( GetPath(), WavData, Format, Freq ) )
        {
            return false;
        }

        alBufferData( Buffer, Format, &WavData[0], static_cast<ALsizei>(WavData.size()), Freq );

        if( OAL::CheckOALError() )
        {
            return false;
        }
    }
    else if( Utils::ToLower(Utils::GetFileExt( GetPath() )) == "ogg" )
    {
        stb_vorbis* File = stb_vorbis_open_filename( GetPath().c_str(), nullptr, nullptr );
        if( !File )
        {
            LOG( ESeverity::Error ) << "Unable to open ogg file: " << GetPath() << "\n";
            return false;
        }
        stb_vorbis_info Info = stb_vorbis_get_info( File );

        int LengthSamples = stb_vorbis_stream_length_in_samples( File ) * Info.channels;
        std::vector<ALshort> Data(LengthSamples);
        stb_vorbis_get_samples_short_interleaved(File, Info.channels, Data.data(), LengthSamples);

        ALenum Flg;
        if( Info.channels == 1 ) Flg = AL_FORMAT_MONO16;
        else if( Info.channels == 2 ) Flg = AL_FORMAT_STEREO16;
        else
        {
            stb_vorbis_close( File );
            LOG( ESeverity::Error ) << "Invalid ogg format: " << GetPath() << "\n";
            return false;
        }

        alBufferData( Buffer, Flg, &Data[0], LengthSamples*sizeof(ALshort), Info.sample_rate );
        if( OAL::CheckOALError() )
        {
            stb_vorbis_close( File );
            return false;
        }

        stb_vorbis_close( File );
    }
    else
    {
        LOG( ESeverity::Error ) << "Invalid audio format\n";
        return false;
    }

    Valid = true;
    return Valid;
}

int COALSoundData::GetSamples() const
{
    if( !IsValid() )
    {
        return 0;
    }

    ALint SizeInBytes;
    ALint Channels;
    ALint Bits;
    alGetBufferi( Buffer, AL_SIZE, &SizeInBytes );
    alGetBufferi( Buffer, AL_CHANNELS, &Channels );
    alGetBufferi( Buffer, AL_BITS, &Bits );
    OAL::CheckOALError();

    return static_cast<int>(SizeInBytes * 8 / (Channels * Bits));
}
