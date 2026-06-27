void main() {
    // Pointer langsung menunjuk ke alamat VGA Buffer di Flat Memory Model
    char* video_memory = (char*) 0xb8000;
    
    // Menulis karakter 'O' dengan atribut warna (putih di atas hitam)
    *video_memory = 'O';
    *(video_memory + 1) = 0x0f;
    
    *(video_memory + 2) = 'S';
    *(video_memory + 3) = 0x0f;
}
