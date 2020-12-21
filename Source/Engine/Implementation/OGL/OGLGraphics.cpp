#include "OGLGraphics.hpp"
#include "OGLFrameBuffer.hpp"
#include "OGLTexture2D.hpp"
#include "OGLVertexBuffer.hpp"
#include "OGLShader.hpp"
#include "OGLUtils.hpp"
#include "../SDLWindow.hpp"
#include "../SDLSurface.hpp"
#include "../SDLFont.hpp"
#include "../../Core/Utils.hpp"
#include "../../Core/Log.hpp"
#include "../../Input/Event.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>

namespace
{
    GLenum ToOGLDepthMode(const EDepthMode Mode)
    {
        switch(Mode)
        {
            case EDepthMode::Never:
                return GL_NEVER;
            case EDepthMode::LessOrEqual:
                return GL_LEQUAL;
            case EDepthMode::Greater:
                return GL_GREATER;
            case EDepthMode::GreaterOrEqual:
                return GL_GEQUAL;
            case EDepthMode::Equal:
                return GL_EQUAL;
            case EDepthMode::NotEqual:
                return GL_NOTEQUAL;
            case EDepthMode::Always:
                return GL_ALWAYS;
            case EDepthMode::Less:
            default:
                return GL_LESS;
        }
    }

    GLenum ToOGLCullMode(const ECullMode Arg)
    {
        switch (Arg)
        {
        case ECullMode::FrontBack:
            return GL_FRONT_AND_BACK;
        case ECullMode::Front:
            return GL_FRONT;
        case ECullMode::Back:
        default:
            return GL_BACK;
        }
    }
}

class COGLImGUIRenderer: public IImGUIRenderer
{
public:
    bool Init() override
    {
        return ImGui_ImplOpenGL3_Init();
    }

    void Update(const float) override
    {
        ImGui_ImplOpenGL3_NewFrame();
    }

    void Draw(ImDrawData* DD) override
    {
        ImGui_ImplOpenGL3_RenderDrawData(DD);
    }

    void Destroy() override
    {
        ImGui_ImplOpenGL3_Shutdown();
    }
};

COGLGraphics::COGLGraphics(CSDLWindow* aWindow):
    Window(aWindow)
{
}

COGLGraphics::~COGLGraphics()
{
    ImGUIRenderer.reset();
    if( Context )
    {
        SDL_GL_DeleteContext( Context );
        Context = nullptr;
    }
}

bool COGLGraphics::Init(const SEngineParams& Parameters)
{
    Context = SDL_GL_CreateContext( Window->GetSDLWindow() );
    if( !Context )
    {
        LOG( ESeverity::Fatal ) << "Unable to create OpenGL context: " << SDL_GetError() << "\n";
        return false;
    }

    if( SDL_GL_MakeCurrent( Window->GetSDLWindow(), Context ) != 0 )
    {
        LOG( ESeverity::Fatal ) << "Unable to setup OpenGL context: " << SDL_GetError() << "\n";
        return false;
    }

    if( SDL_GL_SetSwapInterval( Parameters.VSync ) != 0 )
    {
        LOG( ESeverity::Error ) << "No VSYNC Support: " << SDL_GetError() << "\n";
    }
    else
    {
        LOG( ESeverity::Info ) << "Using VSYNC\n";
    }

    if( !gladLoadGLLoader( SDL_GL_GetProcAddress ) )
    {
        LOG( ESeverity::Fatal ) << "Unable to initialize GLAD\n";
        return false;
    }

    int Max;
    glGetIntegerv( GL_MAX_TEXTURE_SIZE, &Max );
    MaxTextureSize.x = MaxTextureSize.y = Max;

    OGL::CheckErrorOpenGL();

    SetBlendMode( EBlendMode::None );
    SetPolygonMode( EPolygonMode::Fill );
    SetDepthActive( true );
    SetDepthFunction( EDepthMode::Less );
    SetViewport( Rect2( 0.0f, 0.0f, 1.0f, 1.0f ) );
    SetBlendActive( true );
    SetCullMode(ECullMode::Back);
    SetCullActive(false);
    SetFrontFace(EFrontFace::CCW);
    glActiveTexture( GL_TEXTURE0 );

    glEnable(GL_MULTISAMPLE);

    OGL::CheckErrorOpenGL();

    ImGUIRenderer = std::make_unique<COGLImGUIRenderer>();

    LOG( ESeverity::Info ) << "Vendor: " << reinterpret_cast<const char*>(glGetString(GL_VENDOR)) <<
                              " , Type: " << reinterpret_cast<const char*>(glGetString(GL_RENDERER)) <<
                              " , Max Texture Size: " << MaxTextureSize << "\n";
    LOG( ESeverity::Info ) << Utils::Format( "Set Video Mode: %dx%d", Window->GetSize().x, Window->GetSize().y ) << "\n";
    return true;
}

void COGLGraphics::OnEvent(const CEvent& E)
{
    if( E.GetType() == EEventType::WindowSize )
    {
        SEventWindowSize* Data = E.GetData<SEventWindowSize>();
        if( Data )
        {
            // Recalculate Viewport
            SetViewport( GetViewport() );
        }
    }
}

bool COGLGraphics::SaveWindowSurface(const std::string& Path) const
{
    if( Path.empty() )
    {
        LOG( ESeverity::Error ) << "Invalid Path\n";
        return false;
    }

    std::shared_ptr<CSDLSurface> Surface = std::make_shared<CSDLSurface>();
    if( !Surface->Create( Window->GetSize().x, Window->GetSize().y ) )
    {
        LOG( ESeverity::Error ) << "Unable to create surface\n";
        return false;
    }

    glPixelStorei( GL_PACK_ALIGNMENT, 1 );
    if( SDL_MUSTLOCK( Surface->GetSDLSurface() ) )
    {
        SDL_LockSurface( Surface->GetSDLSurface() );
    }

    std::vector<GLubyte> Data( Window->GetSize().x*Window->GetSize().y*4 );
    glReadPixels( 0, 0, Window->GetSize().x, Window->GetSize().y, GL_RGBA, GL_UNSIGNED_BYTE, Data.data() );

    Uint32* SrcPixels = reinterpret_cast<Uint32*>(Data.data());
    Uint32* DstPixels = reinterpret_cast<Uint32*>(Surface->GetSDLSurface()->pixels);
    for(int i = 0; i < Window->GetSize().x; ++i)
    {
        for(int j = 0; j < Window->GetSize().y; ++j)
        {
            DstPixels[j*Window->GetSize().x+i] = SrcPixels[i+(Window->GetSize().y-1-j)*Window->GetSize().x];
        }
    }

    if( SDL_MUSTLOCK( Surface->GetSDLSurface() ) )
    {
        SDL_UnlockSurface( Surface->GetSDLSurface() );
    }
    glPixelStorei( GL_PACK_ALIGNMENT, 4 );

    if( OGL::CheckErrorOpenGL() )
    {
        return false;
    }

    if( IMG_SavePNG( Surface->GetSDLSurface(), Path.c_str() ) != 0 )
    {
        LOG( ESeverity::Error ) << "Unable to Save PNG:" << IMG_GetError() << "\n";
        return false;
    }
    return true;
}

std::unique_ptr<ITexture2D> COGLGraphics::CreateRenderSurface(const ERenderTargetType aTarget, const int aW, const int aH)
{
    auto Ptr = std::make_unique<COGLTexture2D>("", this);
    if (!Ptr->CreateAsRenderSurface(aTarget, aW, aH))
    {
        return {};
    }
    return Ptr;
}

std::unique_ptr<ITexture2D> COGLGraphics::CreateTexture2D(ISurface* Surface)
{
    if (!Surface)
    {
        return {};
    }
    auto Ptr = std::make_unique<COGLTexture2D>( "", this );
    if (!Ptr->CreateFromSurface(Surface))
    {
        return {};
    }
    return Ptr;
}

std::unique_ptr<ITexture2D> COGLGraphics::CreateTexture2D(const std::string& Name)
{
    return std::make_unique<COGLTexture2D>( Name, this );
}

std::unique_ptr<IFont> COGLGraphics::CreateFont(const std::string& Name)
{
    return std::make_unique<CSDLFont>( Name, this );
}

std::unique_ptr<IVertexBuffer> COGLGraphics::CreateVertexBuffer(const std::vector<SVertexElement>& Descriptors, const bool Dynamic)
{
    auto VertexBuffer = std::make_unique<COGLVertexBuffer>();
    if( !VertexBuffer->Create(Descriptors, Dynamic) )
    {
        return {};
    }
    return VertexBuffer;
}

std::unique_ptr<IFrameBuffer> COGLGraphics::CreateFrameBuffer()
{
    auto FrameBuffer = std::make_unique<COGLFrameBuffer>(this, Window);
    if (!FrameBuffer->Create())
    {
        return {};
    }
    return FrameBuffer;
}


std::unique_ptr<IShader> COGLGraphics::CreateShader(const std::string& Name)
{
    return std::make_unique<COGLShader>( Name );
}

void COGLGraphics::Clear()
{
    glClearColor( ClearColor.GetR(), ClearColor.GetG(), ClearColor.GetB(), ClearColor.GetA() );

    GLbitfield Flags = GL_COLOR_BUFFER_BIT;
    if( IsDepthActive() ) Flags |= GL_DEPTH_BUFFER_BIT;

    glClear( Flags );

    OGL::CheckErrorOpenGL();
}

void COGLGraphics::SwapBuffers()
{
    SDL_GL_SwapWindow( Window->GetSDLWindow() );
}

bool COGLGraphics::SetBlendMode(const EBlendMode Mode)
{
    BlendMode = Mode;
    switch(Mode)
    {
    case EBlendMode::Add:
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        break;
    case EBlendMode::Alpha:
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        break;
    case EBlendMode::Mod:
        glBlendFunc(GL_DST_COLOR, GL_ZERO);
        break;
    case EBlendMode::None:
        glBlendFunc(GL_ONE, GL_ZERO);
        break;
    default:
        return false;
    }
    OGL::CheckErrorOpenGL();
    return true;
}

bool COGLGraphics::SetPolygonMode(const EPolygonMode Mode)
{
    PolygonMode = Mode;
    switch(Mode)
    {
    case EPolygonMode::Line:
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
        break;
    case EPolygonMode::Point :
        glPolygonMode( GL_FRONT_AND_BACK, GL_POINT );
        break;
    case EPolygonMode::Fill:
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        break;
    default:
        return false;
    }
    OGL::CheckErrorOpenGL();
    return true;
}

bool COGLGraphics::SetDepthActive(const bool Active)
{
    DepthActive = Active;
    if( DepthActive )
    {
        glEnable( GL_DEPTH_TEST );
    }
    else
    {
        glDisable( GL_DEPTH_TEST );
    }
    OGL::CheckErrorOpenGL();
    return true;
}

bool COGLGraphics::SetDepthFunction(const EDepthMode Mode)
{
    DepthMode = Mode;
    glDepthFunc( ToOGLDepthMode(Mode) );
    OGL::CheckErrorOpenGL();
    return true;
}

bool COGLGraphics::SetViewport(const Rect2& Rect)
{
    Viewport = Rect;
    Vector2 WinSize = Vector2( Window->GetSize().x, Window->GetSize().y );

    glViewport( Viewport.GetX()*WinSize.x, (1.0f-Viewport.GetY()-Viewport.GetHeight())*WinSize.y, 
        Viewport.GetWidth()*WinSize.x, Viewport.GetHeight()*WinSize.y );
    OGL::CheckErrorOpenGL();

    return true;
}

IImGUIRenderer* COGLGraphics::GetImGUIRenderer() const
{
    return ImGUIRenderer.get();
}

void COGLGraphics::SetBlendActive(const bool Arg)
{
    BlendActive = Arg;
    if (BlendActive)
    {
        glEnable(GL_BLEND);
    }
    else
    {
        glDisable(GL_BLEND);
    }
    OGL::CheckErrorOpenGL();
}

void COGLGraphics::SetCullActive(const bool Arg)
{
    CullModeActive = Arg;
    if (CullModeActive)
    {
        glEnable(GL_CULL_FACE);
    }
    else
    {
        glDisable(GL_CULL_FACE);
    }
    OGL::CheckErrorOpenGL();
}

void COGLGraphics::SetCullMode(const ECullMode aMode)
{
    CullMode = aMode;
    glCullFace( ToOGLCullMode(CullMode) );
    OGL::CheckErrorOpenGL();
}

void COGLGraphics::SetFrontFace(const EFrontFace aMode)
{
    FrontFace = aMode;
    if (FrontFace == EFrontFace::CW)
    {
        glFrontFace(GL_CW);
    }
    else
    {
        glFrontFace(GL_CCW);
    }
    OGL::CheckErrorOpenGL();
}