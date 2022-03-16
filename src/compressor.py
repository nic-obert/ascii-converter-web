from typing import Tuple
from ascii_image import ASCIIImage


MULTI_CHAR_SIGN = 0x00
NEWLINE_CHAR_CODE = 10


def decompose_int(number: int, span: int) -> Tuple[int]:
    digest = [0] * span
    for i in range(span):
        digest[i] = number % 256
        number //= 256
        if number == 0:
            break
    return digest
    

def bytes_to_int(data: bytes) -> int:
    return int.from_bytes(data, byteorder='little')


def generate_header(image: ASCIIImage) -> bytes:
    """Return a 5-byte header including metadata for the image."""
    return bytes([
        image.style_code, # 1 byte
        *decompose_int(image.width, 2), # 2 bytes
        *decompose_int(image.height, 2), # 2 bytes
    ])


def extract_header(data: bytes) -> Tuple[int, int, int]:
    """Extract the header from the first 5 bytes of the data."""
    style_code = data[0] # 1 byte
    width = bytes_to_int(data[1:3]) # 2 bytes
    height = bytes_to_int(data[3:5]) # 2 bytes
    return style_code, width, height


def multi_char(char: str, count: int) -> bytes:
    """Return a 3-byte sequence representing a multi-character sequence."""
    return bytes([MULTI_CHAR_SIGN, count, ord(char)])


def compress_ascii_image(image: ASCIIImage) -> bytes:
    # First create an digest with the image header
    digest = bytearray(generate_header(image))
    data = image.data[5:]
    
    count = 1
    current_char = None
    for char in data:

        # Newline character resets the count
        # The last character of an image should always be a newline
        if char == '\n':
            if count < 4:
                digest.extend([current_char] * count)
            else:
                digest.extend(multi_char(current_char, count))
            count = 1
            current_char = None

        elif current_char == char:
            count += 1
            if count == 255:
                digest.extend(multi_char(current_char, 255))
                count = 1
        
        elif count < 4:
            if current_char is not None:
                digest.extend([current_char] * count)
                count = 1
            current_char = char

        else:
            digest.extend(multi_char(current_char, count))
            count = 1
            current_char = char

    return bytes(digest)


def decompress_ascii_image(data: bytes) -> ASCIIImage:
    style_code, width, height = extract_header(data)

    data = data[5:]

    # Create a buffer to store the decompressed image. 
    # Width + 1 is because we need to store the newline character.
    # The buffer is filled with newline characters to ensure that the image has newlines after each row.
    ascii_image = bytearray([NEWLINE_CHAR_CODE] * ((width + 1) * height))

    data_index = 0
    image_index = 0
    while data_index < len(data):
        byte = data[data_index]

        # Check if the end of the row has been reached
        # width - 1 is because indices start at 0
        if image_index % (width - 1) == 0:
            image_index += 1

        # If the byte is a multi-character sequence, read the count and the character
        if byte == MULTI_CHAR_SIGN:
            count = data[data_index + 1]
            char = data[data_index + 2]
            data_index += 3 # Skip the whole multi-char sequence
            ascii_image[image_index:image_index + count] = [char] * count
            image_index += count
        
        # If the byte is a single character, just copy it
        else:
            ascii_image[image_index] = byte
            image_index += 1
            data_index += 1
    
    return ASCIIImage(
        data=ascii_image.decode('ascii'),
        width=width,
        height=height,
        style_code=style_code
    )
