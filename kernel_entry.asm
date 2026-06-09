[org 0x8000]

mov si, message

print_loop:
  lodsb
  cmp al, 0
  je done

  mov ah, 0x0E
  int 0x10
  jmp print_loop

done:
  jmp $

message db 'Hello from kernel!', 0
times 512-($-$$) db 0
