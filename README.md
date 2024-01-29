# Clicker

Clicker is a simple auto-clicker program that simulates mouse clicks on Linux using the X11 library.

## Usage

1. Clone the repository:

   ```bash
   git clone https://github.com/carlooosdev/clicker.git
   ```

2. Compile the code using gcc:

   ```bash
   gcc -o clicker clicker.c `pkg-config --cflags --libs gtk+-3.0` -lX11 -lXtst
   ```

3. Run the executable:

   ```bash
   ./clicker
   ```

4. The Clicker window will appear with a toggle button labeled "Auto Clicker" and a secondary menu option named "Set Click Interval".

5. Click on the "Auto Clicker" toggle button to enable or disable the auto-clicking feature.

6. Right-click on the toggle button to open the context menu and select "Set Click Interval" to configure the time interval between each mouse click.

7. In the "Set Click Interval" dialog, enter the desired interval in milliseconds and click "Ok" to save the changes or "Cancel" to discard them.

8. The auto-clicker will start clicking at the specified interval whenever the toggle button is enabled.

9. To stop the auto-clicking, simply disable the toggle button.

## Notes

- The default click interval is set to 1000 milliseconds (1 second). You can change this interval using the "Set Click Interval" option in the context menu.
- The auto-clicker only simulates left mouse button clicks.
- The auto-clicker works by interacting with the underlying X11 system, so it may not have the same behavior across different Linux distributions or desktop environments.
