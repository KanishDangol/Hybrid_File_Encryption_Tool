# File Encryption Tool (AES + RSA Hybrid Scheme)

## Overview

This project is a simple yet effective **file encryption tool** developed in C with a user-friendly **GTK-based GUI**. It enables users to securely encrypt and decrypt files on their local machine using a **hybrid cryptographic approach**, combining the speed of symmetric encryption (AES-256) with the security advantages of public-key infrastructure (RSA).

## Key Features

- 🔐 **AES-256 Encryption/Decryption** in 16-byte blocks
- 📁 Encrypts **text files** 
- 🧑‍💻 **User-friendly GTK GUI** with file browsing, encryption, and decryption actions
- ✅ Displays encryption/decryption status messages
- 🔑 RSA public and private keys included for hybrid scheme key management

## How It Works

1. **AES Key Generation**  
   A fresh, random 256-bit (32-byte) AES key is generated for each encryption session using `/dev/urandom`.

2. **RSA Integration**  
   While the tool primarily uses AES for file encryption, an RSA key exchange mechanism is incorporated to add an extra layer of security to protect the AES key. This architectural touch aligns with hybrid cryptography best practices.

3. **File Encryption**  
   The input file is encrypted in 16-byte blocks using the generated AES key and saved as a `.enc` file. Padding is applied to the final block if needed.

4. **File Decryption**  
   The `.enc` file is decrypted using the same AES key and recovered to its original state (stored as `.dec` file).

> For example:  
> `input.txt` → `input.txt.enc` → `input.txt.enc.dec`

## How to Run the Program

### Prerequisites

Ensure the following libraries are installed:
- `libgmp-dev`
- `libgtk-3-dev`

You can install them with:
```bash
sudo apt install libgmp-dev libgtk-3-dev
```

### Build the Program

Use the provided Makefile:
```bash
make clean
make
```

### Command Line Usage

Encrypt a file:
```bash
./hybrid encrypt input.txt input.txt.enc
```

Decrypt a file:
```bash
./hybrid decrypt input.txt.enc input.txt.enc.dec
```

### GUI Usage

Launch the graphical interface with:
```bash
make run-gui
```

Then:
- Click **Browse** to select your file
- Click **Encrypt** to create a `.enc` file
- Click **Decrypt** to generate the original as `.dec`

<img width="501" height="337" alt="Screenshot 2025-10-15 135719" src="https://github.com/user-attachments/assets/4a7fe86e-2f79-4382-88c4-59993af95f73" />

  

## Technologies Used

- **C (GCC)** for low-level cryptographic implementation  
- **GTK+3** for cross-platform graphical interface  
- **GMP Library** for large-number support in RSA  
- **AES-256** (custom implementation)  
- **RSA key generation** for hybrid encryption environment

## Conclusion

This tool offers a practical demonstration of file security using symmetric cryptography, wrapped in the architecture of a hybrid scheme. It provides a secure, efficient, and easy-to-use interface for local file encryption.
