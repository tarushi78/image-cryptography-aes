# Image Cryptography using AES-128

Built this as a college project to explore how encryption actually works 
at an implementation level — not just theory.

## What it does
Encrypts and decrypts images using AES-128 in CBC mode. Has a basic Win32 
GUI, logs activity to a MySQL database, and can transfer encrypted images 
between two systems over TCP sockets.

## Tech used
- C++ (core logic)
- AES-128 CBC mode (manual implementation)
- Win32 API (GUI)
- MySQL (logging)
- TCP Sockets (file transfer)

## Files
- `aes.cpp/h` — encryption/decryption core
- `Encrypt.cpp/h` — encryption module
- `sender.cpp/h` — TCP sender
- `receiver.cpp/h` — TCP receiver  
- `database.cpp/h` — MySQL logging
- `gui_glut.cpp/h` — GUI
- `main_desktop.cpp` — entry point

## Notes
Ran into a fun bug with 32-bit vs 64-bit MinGW and MySQL connector 
mismatch — took a while to fix but learned a lot from it.
