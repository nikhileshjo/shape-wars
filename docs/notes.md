# Notes
- Normalizing a vector has a division component to it, so if the vector has a magnitude of 0, a unit vector is undefined, so if your transformation depends on it the shape won't render as the positon becomes undefined. In my case, it would happen on a key release because a key release sets that direction's value to 0, so if the resulting vector becomes `0i + 0j` its magnitude became 0, which produced `nani + nanj` (not a number). To enusure this doesn't happen, I've set the vector normaization to return `0i + 0j` if the magnitude is 0.
- The game loop always requires you to take inputs i.e. you need to run `m_window.pollEvent()` in everyframe whether you require input or not at that time. Failing to do so would mean the game window isn't draining the event window, from the OS's perspective it ignores all inputs from the OS, meaning, it's stopped responding and will prompt you to force close or wait for the window to become responsive. How do I know this? While testing, I just needed to print my shape on the screen, so I didn't use `pollEvent()` and my window would crash all the time with no failures to investigate.
- We can update shape.setPosition() in `sRender()` or in `sMovement()`, but we choose to do it in `sRender()` that way, `sMovement()` doesn't concern itself with any rendering logic and we have now decoupled it compeletely.
- How to handle object types with no default constructors? I had issues declaring sf::Text as it doesn't have a default constructor, so I can't do `sf::Text m_text;`, I need it to be accessible to other members of the class to in the GameEngine class. So we can use std::optional, this allows us to declear a variable that doesn't have a default constructor and also we can intialize it later. Syntax: `std::optional<sf::Text> m_text;` the rest is the same as any other variable then we can use it later as `m_text.emplace(m_font, "", m_textConfig.size);`. One thing to remember, this creates a pointer and not the object itself, so anything you do needs to be de-referenced or pointed to, example `m_text->setString(myString);` or `(*m_text).setString(myString);` and not `m_text.setString(myString)`.
- Had an issue with my sMovement function. All my flag checks were an individual if, so what this did was, within the same frame, more than one if would get executed for x or y coordinate, since "down" checks where after the "up" checks, it would see that "down" is set to false and set the y back to 0 right after the "up" check set it to true in case you pressed up, same for left and right. Below is the faulty code. Let's see what happens when you press up.
```
    if ( p->get<CInput>().up)  // this becomes true and velocity y is set to -1
    {
        pVel.y = (-1);
    }
    if (!p->get<CInput>().up)
    {
        pVel.y = 0;
    }

    if ( p->get<CInput>().down )
    {
        pVel.y = 1;
    }
    if (!p->get<CInput>().down) // in the same frame this too is true so, it sets y back to 0
    {
        pVel.y = 0;
    }
    if ( p->get<CInput>().left )
    {
        pVel.x = (-1);
    }
    if (!p->get<CInput>().left)
    {
        pVel.x = 0;
    }

    if ( p->get<CInput>().right )
    {
        pVel.x = 1;
    }
    if (!p->get<CInput>().right)
    {
        pVel.x = 0;
    }
```
1. As you might have noticed, from above, it doesn't let the entity go left or up in this case.\
2. How do we resolve this? We make x changing if statements as one if-else block and y changing statement as one if-else block, that way, if anything changes the state of x in the frame, we ensure that in the same frame it doesn't get overwritten by another x editing statment, same goes for y.\
3. Why not put it all in one if-else block? Because if we did, we'll lose diagonal directions for one frame. At a time only one of the if-else block would get activated, so there will be an input delay if 2 dirction keys were pushed.
4. Order matters: We will prioritise key press over key release, so our if-else would have first 2 conditions that check for a up or down/left or right being true, if they are, they'll be execute, else we'll stop moving. This creates a small issue, though. We always prioritize one side over other, like in my implementation, we prioritize up over down/ left over right, if both up and down are pressed, regardless of the order of pressing, we'll always choose up.

* Ran into error of duplicate definition of function. Why did it happen, I was importing a header file in 2 different scripts. When compiling a C++ project, the first step in the compiler is to get all the #includes in a file and make them a single file, and then make once single concat file from all the files, what this means is, if you included the same file twice or more in your project you essetially are duplicating your code or you could say you define the same thing again. To avoid this, I refactored the code with .h files containing only the declarations and .cpp containing their defintions and any implementations. Also, use `#pragma once` at the top of your header files, this means in a single project even if you import that file twice somewhere, it is considered only once in the entire project (which is sufficient). As we import only header files, we add them only in .h files.

* The above refactoring caused another issue, template functions became invalid because, as they are defintions I placed them in .cpp files, which is wrong. A template is only valid within the same file. Although, it can accept any datatype inputs even from other files importing them, but the declaration and defintion must be in the same file as shown in the file: [Entity.h](../src/Entity.h)
* If your variable is passed as a constant but then it relies of a function that's not "declared" or "defined" constant, it'll cause a compiler error. It doesn't matter if you don't really change anything inside it (if you did though, that would cause a compiler error too), if it's not declared constant, it compiler won't know it doesn't do anything to the inputs.
* For bullets, we spawn them at the center of the player. If you see the function spawnBullet(), we spawn the bullet at the center of the player with default origin i.e. top left corner, yet we don't see the bullet spawn off-center, why? **Check why**
* Problems with `sDebugger()` I faced were, I couldn't get it to render, because I was rendering the imgui window before the `m_window.clear()` which clears the previous screen. To resolved this, I moved the rendering from sDebugger() function to sRender() (this is better anyways as it puts rendering exactly where it belongs) right before the `m_window.display()`.
* I didn't assign m_event with pollevents in the debugger, and did this instead:
```
while(m_event)
{
    ImGui::SFML::ProcessEvent(m_window, *m_event);
}
```
Why? Because I thought the assignment in the `sUserInput()` was doing the job and had declared a member variable to pass them to both the functions. But later realized that they can independently take their own events and didn't need a common member variables to pass values between them.
* There is one issue though, if you click on the imgui window, it registers as an input to the player too. [Check for a fix].
* We only need `ImGui::SFML` when we need to pass something from SFML to ImGui, otherwise all our Imgui APIs are valid. Example, passing the window object. But, creating a tab we use the usual `ImGui::BeginTabBar()`.
* pass event to imgui in the `sUserInput()` funtion and not in `sDebugger()`, because it can cause issues in responding as there might be delays in responding.
* if you have a std::string type and you want it to be a char pointer/array, use c_str(), example: `myStr.c_str()`