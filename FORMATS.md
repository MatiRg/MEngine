# File Formats
Formats used by Engine.
## Config files that can be used by CConfig class
XML Data format
### Structure
```xml
<Category>
    <Name> Value </Name>
    ...
</Category>
...
```
Where Value can be: std::string, int, float. bool, Rect2, Vector2, Vector3 and Color
### Special Case
Config file used by Engine:
* Width - Width of the Window
* Height - Height of the Window
* Caption - Caption of the Window
* VSync - Use VSync
* UseAudio - Use Audio
* Ticks - Ticks per frame