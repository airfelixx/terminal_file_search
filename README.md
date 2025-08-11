# Simple File Navigator

This is a small command-line program I wrote as a learning exercise. It helps you search for files by name keyword inside a directory, then lets you navigate through the results, open files for editing, or delete them, all from the terminal.

---

## Features

- Input a directory path and a keyword.
- Recursively search for files whose names contain the keyword.
- Store found files in a circular linked list.
- Navigate through files circularly using simple keyboard commands.
- Open files in your system’s default text editor.
- Delete files directly from the program.
- Commands:
  - `n` — move to the next file
  - `o` — open the current file for editing
  - `d` — delete the current file
  - `e` — exit the program

---

## How to Use

1. Run the program.
2. Enter the directory path you want to search inside.
3. Enter the keyword to search for in filenames.
4. Use the commands above to navigate, open, delete, or exit.

---

## Notes

- This was a quick project built over a few hours as part of my coding practice.
- It’s a simple tool, mainly intended to help me learn about file handling, linked lists, and basic terminal UI.
- Use with caution: deleted files are permanently removed.

---

## Requirements

- C++17 compatible compiler (for `std::filesystem` support).
- A Linux/Unix-like environment for the terminal interface.
- Default text editor configured (e.g., `gedit`).

---

## License

This project is free to use and modify. No warranty provided.

---

Thanks for checking it out! Feel free to open issues or suggest improvements.
