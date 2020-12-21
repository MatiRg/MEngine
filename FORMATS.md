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

## Material Files - *.mat
XML Data format
### Structure
```xml
<Material>
	<Shader Name="Value">
		<Define>Value</Define>
		...
	</Shader>
	<Variable Name="Name" Type="Type">Value</Variable>
	...
</Material>
```
Shader only one per Material.
Name shader name with extension --> Diffuse.shader
Shader can have none or many defines that costumize shader example Lit, Diffuse Texture, Specular Texture 
Variables can be any number or none. Variables that are updated can be ignored.
Variable Parameters:
* Name is param name in Shader.
* Type is one of these: Int, Float, Vector2, Vector3, Vector4, Color, Texture
Variables that are predefined and must be ignored in Material File:
* Model
* View
* Projection
* CameraPosition