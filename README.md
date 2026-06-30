# 🔐 Image Cryptography using AES-128

Built this as a college project to explore how encryption actually works at an implementation level — not just theory.

## What it does

Encrypts and decrypts images using AES-128 in CBC mode — implemented from scratch without any crypto library. Has a basic Win32 GUI, logs every transfer to a MySQL database, and sends encrypted images between two systems over TCP sockets — verified using Wireshark.

## 🛠️ Tech Stack
- **C++** — core logic
- **AES-128 CBC** — implemented from scratch without any crypto library
- **Win32 API** — GUI
- **MySQL** — activity logging
- **TCP Sockets** — encrypted file transfer

## 📂 Files
| File | Purpose |
|------|---------|
| `aes.h` | AES-128 encryption/decryption core |
| `Encrypt.cpp/h` | Encryption module |
| `sender.cpp/h` | TCP sender |
| `receiver.cpp/h` | TCP receiver |
| `database.cpp/h` | MySQL activity logging |
| `gui_glut.cpp/h` | Win32 GUI |
| `main_desktop.cpp` | Entry point |

## 🔍 How it Works
1. Select an image from the GUI
2. Enter an encryption key
3. Image gets encrypted → AES-128 CBC mode
4. Send over TCP to receiver
5. Receiver decrypts using same key
6. Transfer logged to MySQL with timestamp

## 🚀 How to Run

> Tested on Windows with MinGW-w64 and MySQL 8.0

```bash
g++ main_desktop.cpp sender.cpp receiver.cpp database.cpp gui_glut.cpp -o ImageCrypto -lws2_32 -lmysqlclient -lgdi32
./ImageCrypto
```

> Note: MySQL Connector/C++ and MinGW 64-bit required.
> Exact compile flags may vary by setup.

## 🐛 Interesting Bug
Ran into a tricky 32-bit vs 64-bit MinGW and MySQL connector mismatch that took a while to debug — but that's honestly where most of the learning happened.

---
*Part of my 5th semester mini project — open to feedback!*
