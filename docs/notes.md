# Notes
- Normalizing a vector has a division component to it, so if the vector has a magnitude of 0, a unit vector is undefined, so if your transformation depends on it the shape won't render as the positon becomes undefined. In my case, it would happen on a key release because a key release sets that direction's value to 0, so if the resulting vector becomes `0i + 0j` its magnitude became 0, which produced `nani + nanj` (not a number). To enusure this doesn't happen, I've set the vector normaization to return `0i + 0j` if the magnitude is 0.
- The game loop always requires you to take inputs i.e. you need to run `m_window.pollEvent()` in everyframe whether you require input or not at that time. Failing to do so would mean the game window isn't draining the event window, from the OS's perspective it ignores all inputs from the OS, meaning, it's stopped responding and will prompt you to force close or wait for the window to become responsive. How do I know this? While testing, I just needed to print my shape on the screen, so I didn't use `pollEvent()` and my window would crash all the time with no failures to investigate.
- We can update shape.setPosition() in `sRender()` or in `sMovement()`, but we choose to do it in `sRender()` that way, `sMovement()` doesn't concern itself with any rendering logic and we have now decoupled it compeletely.
- How to handle object types with no default constructors? I had issues declaring sf::Text as it doesn't have a default constructor, so I can't do `sf::Text m_text;`, I need it to be accessible to other members of the class to in the GameEngine class. So we can use std::optional, this allows us to declear a variable that doesn't have a default constructor and also we can intialize it later. Syntax: `std::optional<sf::Text> m_text;` the rest is the same as any other variable then we can use it later as `m_text.emplace(m_font, "", m_textConfig.size);`
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