# LSB Image Steganography

## 📌 Project Overview

LSB (Least Significant Bit) Image Steganography is a C programming project that hides secret data inside a BMP image using the Least Significant Bit technique.

The project provides two operations:

- **Encoding:** Hides secret file data inside a BMP image.
- **Decoding:** Extracts the hidden data from the encoded image.

The encoded image looks visually similar to the original image, while carrying hidden information.

## ✨ Features

- Encode secret files into BMP images.
- Decode hidden information from BMP images.
- Check image capacity before encoding.
- Store the secret file extension and file size.
- Identify encoded images using a magic string (`#*`).
- Support custom output image and decoded file names.
- Preserve the BMP header and remaining image data.

## 🛠️ Technologies Used

- Programming Language: C
- GCC Compiler
- Bitwise Operators
- File Handling
- Pointers and Structures
- Command-Line Arguments
- Binary File Operations
- LSB Steganography

## 📂 Project Structure

```text
LSB-Image-Steganography/
│
├── main.c
├── encode.c
├── encode.h
├── decode.c
├── decode.h
├── common.h
├── types.h
├── beautiful.bmp
├── secret.txt
|
├── outputs/
│   ├── encoding_output.png
│   ├── decoding_output.png
│   ├── original_image.png
│   ├── stego_image.png
│   └── recovered_text.png
│
└── README.md
```

## ⚙️ Working Process

### 1. Encoding Process

The encoding process hides the secret file inside the source BMP image.

Steps:

1. Open the source image and secret file.
2. Check whether the image has enough capacity.
3. Copy the 54-byte BMP header.
4. Encode the magic string (`#*`).
5. Encode the size of the secret file extension.
6. Encode the secret file extension.
7. Encode the size of the secret data.
8. Encode the secret data into image bytes using LSB.
9. Copy the remaining image data into the output image.

### 2. Decoding Process

The decoding process retrieves the hidden data from the BMP image.

Steps:

1. Open the encoded BMP image.
2. Check for the magic string (`#*`).
3. Decode the secret file extension size.
4. Decode the secret file extension.
5. Validate the output file extension.
6. Decode the secret data size.
7. Extract the hidden data.
8. Save the recovered data into an output file.

## 💻 Compilation

Compile the source files using GCC:

```bash
gcc *.c
```

Run the executable:

```bash
./a.out
```

## 🚀 Execution

### Encoding

**Command:**

```bash
./a.out -e beautiful.bmp secret.txt
```

The encoded image is created with the default name `stego.bmp`.

To specify a custom output image name:

```bash
./a.out -e beautiful.bmp secret.txt output.bmp
```

### Decoding

**Command:**

```bash
./a.out -d beautiful.bmp stego.bmp
```

The program searches the supplied BMP files for the encoded image and saves the recovered data as `output.txt` by default.

To specify a custom output file name:

```bash
./a.out -d stego.bmp recovered.txt
```

## 📸 Project Outputs

### Original Image

The original BMP image used for encoding.


<img width="366" height="274" alt="original_image" src="https://github.com/user-attachments/assets/70b59c6a-d299-43e1-8ae7-a239bc94da64" />




### Encoding Output

Successful encoding of secret data into the BMP image.

<img width="526" height="310" alt="encoding_output" src="https://github.com/user-attachments/assets/ce93e748-4a14-43cf-962c-539f45c69340" />




### Stego Image

The generated BMP image containing hidden information.


<img width="363" height="290" alt="stego_image" src="https://github.com/user-attachments/assets/b036017a-9b44-4796-bbf4-fe886fc9f39a" />



### Decoding Output

Successful extraction of the hidden information.


<img width="452" height="224" alt="decoding_output" src="https://github.com/user-attachments/assets/1787a954-b12a-4d78-89d7-9663d35348d5" />




### Recovered Secret Data

The extracted content is saved into a text file.

<img width="174" height="45" alt="recovered_secret_text" src="https://github.com/user-attachments/assets/9a1ea1b1-4529-492a-9c7a-4acd95a68e53" />


## 🎯 Learning Outcomes

Through this project, I gained practical experience in:

- Understanding the Least Significant Bit technique.
- Performing bitwise operations for encoding and decoding.
- Handling BMP images using binary file operations.
- Working with file pointers and structures.
- Implementing file validation and command-line arguments.
- Understanding how secret information can be embedded in and extracted from images.

## 🔮 Future Enhancements

- Support additional image formats.
- Improve error handling and input validation.
- Support larger secret files.
- Add encryption before encoding secret data.
- Improve the security and robustness of the application.

## 👩‍💻 Author

**Sai Keerthi**

GitHub: [Saikeerthi14](https://github.com/Saikeerthi14)
