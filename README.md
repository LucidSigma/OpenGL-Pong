# OpenGL-Pong
 A simple pong clone made with OpenGL and C++17.
 Was made to test my OpenGL skills before moving onto 3D stuff and Vulkan.

## Controls
 **W**/**S** - move left paddle.
 **Up**/**Down** - move right paddle.
 **Enter** - begin game.
 **Escape** - quit game.
 **F2** - take screenshot.

## Credits
 All textures made with [Paint.NET](https://www.getpaint.net/).
 All sound effects make with [as3sfxr](https://www.superflashbros.net/as3sfxr/).
 All fonts from [dafont](https://www.dafont.com/nasalization.font).

 Music from https://filmmusic.io
 "Edm Detection Mode" by Kevin MacLeod (https://incompetech.com)
 License: CC BY (http://creativecommons.org/licenses/by/4.0/)

## Dependencies
 [SDL2](https://www.libsdl.org/index.php) - for window creation, input handling and OpenGL context creation.
 [OpenGL](https://www.opengl.org/) - for graphics.
 [GLAD](https://glad.dav1d.de/) - for accessing modern OpenGL functions.
 [GLM](https://glm.g-truc.net/0.9.9/index.html) - for graphical mathematics.
 [STB Image](https://github.com/nothings/stb) - for loading to/writing from image files.
 [OpenAL Soft](https://github.com/kcat/openal-soft) - for audio.
 [FreeType](https://www.freetype.org/) - for converting TTF files into texture atlases to be rendered.
 [tinyxml2](https://github.com/leethomason/tinyxml2) - for reading XML files.

## Building
 This project was made for personal usage and thus does not have a universal build system.
 However, the Visual Studio 2019 solution is in the repository and can be used to build the project on Windows.