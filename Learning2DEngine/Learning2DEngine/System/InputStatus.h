#pragma once
namespace Learning2DEngine
{
    namespace System
    {
        enum InputStatus
        {
            // The key or mouse button was released.
            KEY_UP,
            // The key or mouse button was pressed.
            KEY_DOWN,
            // The key was held down.
            KEY_HOLD
        };
    }
}