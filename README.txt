Before compiling, please install the newest version of SOIL library by running the command:

sudo apt-get install libsoil-dev

The libsoil should then be installed to your /usr/include/ directory; you can find it by running the command:

dpkg -L libsoil-dev | grep include

then if it's successfully instaled, it should show:
/usr/include
/usr/include/SOIL
/usr/include/SOIL/SOIL.h
/usr/include/SOIL/image_DXT.h
/usr/include/SOIL/image_helper.h
/usr/include/SOIL/stbi_DDS_aug.h
/usr/include/SOIL/stbi_DDS_aug_c.h
/usr/include/SOIL/stb_image_aug.h

Next, compile the code by typing "make"

After compiling, run the code by typing "./mhu9_mp3"

A teapot with a texture and an environmental reflected on it (I used multitextual mapping) should appear first and it should start rotating, with light on it.

Please press any of the following keys to change the texture mapping, environmental mapping, and light, etc:

Note: I used two environment images for the background. Please check the image "norway.jpg" or "winter.jpg" in this folder to check if they correctly map to the teapot. 

Press 'E' to change the surronding environment (change "norway.jpg" to "winter.jpg" or vice versa).
Press 'T' to change the teapot's texture.
Press 'L' to turn off/on the light source (two light sources used, turned off/on at the same time)
Press 'R' to stop it from rotating
Press '+' or '=' to zoom in.
Press '-' or "_" to zoom out.
Press arrow keys to manually move the teapot.
Press 'R' to restart rotating.
Press 'Esc' to exit the program.

Note: you can press 'C' to let it only show the texture, and 'C' again to retrieve the environment mapping.

Citation:
the equation I used to calculate the arbituary axis rotation comes from:
http://inside.mines.edu/~gmurray/ArbitraryAxisRotation/ArbitraryAxisRotation.html
