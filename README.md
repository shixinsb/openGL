[summary](https://www.dropbox.com/s/fogb8huysbt53vp/openGL.pdf?dl=0)
# [openGL](https://www.opengl.org/)
this is beginner practice of openGL. The tutorial is on
[learnopengl](https://learnopengl.com/ )
## dependencies
  * GLFW 
  * GLEW
  * SOIL

### [GLFW]( http://www.glfw.org/download.html )
Static library to create an OpenGL context, define window parameters and handle user input. Retrieving, building and linking the GLFW library.

      $ sudo apt-get install cmake cmake-gui
      $ cmake-gui
      // change to global lib
      $ cmake ..
      $ sudo cmake install
### [GLEW]( http://glew.sourceforge.net/index.html ) 
Static library to manages all that cumbersome work.
       
        $ make 
        $ sudo make install

### [SOIL]( http://www.lonesock.net/soil.html)
an image-loading library that supports several popular formats and does all the hard work for us.
     
     // in ~/projects/makefile change LOCAL
     $ make
     $ sudo make install


### [GLM]( http://glm.g-truc.net/0.9.5/index.html)
GLM stands for OpenGL Mathematics and is a header-only library. It contains some useful linear algebra operations.

      $ cmake-gui
      // change to global lib
      $ cmake ..
      $ sudo cmake install
