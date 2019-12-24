# ALIEN ENGINE

Alien Engine is a Game Engine made by Víctor Segura Blanco and Oriol Capdevila Salinas, two studens from CITM University, Terrassa.
The user can check the editor options activating them in the renderer window.

## Github Links

* [Alien Engine Repository](https://github.com/VictorSegura99/Alien-GameEngine)
* [Alien Engine Releases](https://github.com/VictorSegura99/Alien-GameEngine/releases)
* [Víctor Segura Blanco Github](https://github.com/VictorSegura99)
* [Oriol Capdevila Salinas Github](https://github.com/OriolCS2)

## Controls

* Right Click + W/A/S/D: For Movement
* Right Click + Z/X: For Going Up/Down
* Left Alt + Left Click: Rotate
* Left Alt + Middle Click: Move with mouse
* F: Focus the selected object
* L: Look the selected object
* Mouse Wheel: Zoom In/Out
* W: Translate Mode
* E: Rotate Mode
* R: Scale Mode
* Shift + W: GLOBAL TRANSFORM MODE
* Shift + L: LOCAL TRANSFORM MODE

## Shortcuts

* Left Ctrl / Right Ctrl + A: Panel About
* Left Ctrl / Right Ctrl + I: Panel Project
* Left Ctrl / Right Ctrl + O: Panel Config
* Left Ctrl / Right Ctrl + T: Panel Console
* Left Ctrl / Right Ctrl + R: Panel Render
* Left Ctrl / Right Ctrl + U: Panel Hierarchy
* Left Ctrl / Right Ctrl + P: Panel Create Object
* Left Ctrl / Right Ctrl + M: Panel Inspector
* Left Ctrl / Right Ctrl + E: Panel Scene
* Left Ctrl / Right Ctrl + D: Panel ImGui Demo
* Left Ctrl / Right Ctrl + B: Edit Layouts
* Left Ctrl / Right Ctrl + G: Panel Game

* Left Ctrl / Right Ctrl + S: Save Scene
* Left Ctrl / Right Ctrl + N: New Scene
* Left Ctrl / Right Ctrl + L: Load Scene
* Left Alt / Right Alt + S: Save Scene as...

* Left Ctrl / Right Ctrl + Z: Reverse Action
* Left Ctrl / Right Ctrl + Y: Advance Action

* Left Ctrl / Right Ctrl + D: Clone Selected Objects

* Left Alt / Right Alt + F1: Report Bug

* F3: Mesh View
* F4: Wireframe Mode
* F5: View Grid
* F6: View Normal Vertex
* F7: View Normal Face
* F8: View Outline
* F9: View AABB
* F10: View OBB
* F11: View Octree

## Innovations

### Assigment 1

* Layouts: Personalized layouts that you can create, remove or edit, and save separately
* Shortcuts System: can be customized in Panel Config
* Outline: similar to Unity, parent and children different colors
* Scene inside a ImGui Window
* Docking
* Component Light
* Everything customazible at Panel Render
* Component Material Opacity and Loaded Textures Selectors
* Hierarchy System Done and Drag and Drop in Hierarchy Panel

### Assigment 2

* Control + Z (Undo)/ Control + Y (Redo): System that allows the user to go back actions made and then, if the user doesn't change the timeline, then can recover the same actions they just got back. This line is respected between the scene and the prefab scene also, so you can undo and redo actions individually between scenes and the timelines don't get lost.

* Prefabs: You can save models with all their changes in a prefab, so the user is allowed to save concrete models and presets linked to the original. You also can lock the prefab so if you change the original prefab, the locked one stays as a variant. You also can rewrite the original prefab and make the locked one the original or the inverse, make the locked one as the original. To Open the prefab and go to their particular scene the user can double-click on the file in the explorer window or open prefab in inspector or right click in hierarchy. The user can create a prefab dragging it inside the prefab folder in the explorer or its children, or making it a prefab with the right click menu.

* Explorer Window: The user can manage all folders and assets within the explorer window as they please, like create folders, delete folders and assets, reorganize folders and assets, renames, drag&drops to scene, etc.

* Scene Management: The user can load, save and create scenes with a window explorer.

* Mini Camera Preview in scene: little window in scene that appears if the user selects a game object with a camera to allow them to see how it would look in the game.

* Copy&Paste Components: You can copy a component an paste it in an existing one of the same type and all the data gets rewrited.

* Panel Game: Panel where the scene gets printed from the selected game camera view. It's independent from the scene panel so the user can visualize them at the same time.

* Light and Camera Icons in Scene: Icons that show where this components are and help in the mousepicking of them.

* Octree: To view octree, check box in the panel renderer in the octree options must be active and at least one gameobject with mesh must be static. 

### Assigment 3

* Intro Icon Image while opening the engine with a progress bar.

* More than 1 object is supported to be selected, cntrl to select another keeping the others. Also they can be moved/rotated/scaled or deleted and cntrl D clone them. Moving a object with shift pressed, clones it and move the clone.

* GameObjects have a tag and can be assigned in the inspector. Also there is a menu to create or remove tags. GameObjects can be find with their tag.

* There is a text editor for the scripts in the engine.

* Clone GameObjects

* Engine can be started with game mode. Some modules and ImGui is never created.

* Maybe HotReload?

## Panels

* Panel Configuration: Options to change name, max fps and see information about hardware and framerate. Also there you can 
customize shortcuts, windows style, the window size and mouse inputs. 
* Panel Console: Here you can see errors and information about what you load.
* Panel Scene: Simple panel with just the scene inside.
* Panel Inspector: Shows the components information of the selected object.
* Panel Hierarchy: Shows the hierarcchy of the all game objects that are in the scene, here you
can change parents.
* Panel Render: Here you can customize mesh, background, grid, normal of vertex and faces, outline... changing the color, the line width, length...
* Panel Create Object: Panel with options to select and create the primitive object you want.
* Panel Layout: Two types of panels, one to create a new layout and the other with info about all layouts (here you
can remove or rename layouts).
* Panel About: Base info about creators of this software and libraries used.

## Tasks by Member Assigment 2

* Oriol Capdevila:

	* Mouse Picking
	* Octree, octree in camera culling & in mouse picking
	* Resource Manager
	* The Explorer
	* Hierarchy and reparent
	* Components system
	* Control Z & Control Y (in almost every action)
	* Prefabs
	* Serialize scene 
	* Changing between scenes (with engin explorer or window explorer)
	* Library created again if it doesn't exists
	* Start, pause and tick 
	* Primitives as resource
	* All right click menus
	* Static options in gameobjects and alert popups

* Víctor Segura:

	* AABB and OBB
	* Camera as component
	* Frustum 
	* Frustum Culling
	* Panel Game
	* Panel Preview Camera
	* Scene Guizmos
	* Engine Icons
	* Camera and Light 3D icons in scene

## License

MIT License

Copyright (c) 2019 Víctor Segura Blanco & Oriol Capdevila Salinas

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
