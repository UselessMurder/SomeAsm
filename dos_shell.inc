;*dos_shell*
macro push [arg] {push arg}
macro pop [arg] {pop arg}
macro clr [arg] {xor arg,arg}

macro long_loop target 
{
    local met,exit
    loop met
    jmp exit
  met:
    jmp target
  exit:
}

macro set_seg reg,seg
{
  push eax
  mov ax,seg
  mov reg,ax
  pop eax
}

macro print_string str_addr 
{
  push edx,eax
  mov dx,str_addr
  mov ah,9
  int 21h
  pop eax,edx
}

macro input_string in_addr,in_size,out_addr,out_size 
{
  push esi,edx,eax
  mov ah,[in_size]
  mov byte[in_addr+1],0
  mov byte[in_addr],ah
  mov ah,0ah
  lea dx,[in_addr]
  int 21h
  movzx ax,byte[in_addr+1]
  lea si,[in_addr+1]
  inc ax
  mov byte[out_size],al
  add si,ax
  mov byte[si],'$'
  add dx,2
  mov word[out_addr],dx
  pop eax,edx,esi
}

macro compare_string first_addr,first_size,second_addr,second_size,result
{
    local compare_begin,compare_false,compare_end
    push ecx,edx,eax,esi,edi
    mov al,byte[first_size]
    mov ah,byte[second_size]
    mov si,first_addr
    mov di,second_addr
    mov byte[result],1
    cmp al,ah
    jne compare_false
    movzx cx,al
  compare_begin:
    mov dl,[si]
    mov dh,[di]
    cmp dl,dh
    jne compare_false
    inc si
    inc di
    loop compare_begin
    jmp compare_end
  compare_false:
    mov byte[result],0
  compare_end:
    pop edi,esi,eax,edx,ecx
}

macro atoi_eax str_addr,str_size,result
{
    local lp,error,exit
    push ecx,edx,ebx,esi,edi
    mov byte[result],1
    mov si,str_addr              
    mov edi,10               
    movzx cx,byte[str_size]             
    jcxz error       
    clr eax,ebx                            
    lp:
    mov bl,[si]             
    inc si                
    cmp bl,'0'             
    jl error          
    cmp bl,'9'             
    jg error        
    sub bl,'0'              
    mul edi                  
    jc error         
    add eax,ebx             
    jc error         
    loop lp           
    jmp exit          
  error:
    clr eax               
    mov byte[result],0                    
  exit:
    pop edi,esi,ebx,edx,ecx
}

macro atoi_ax str_addr,str_size,result
{
    local exit
    push ebx
    mov byte[result],1
    atoi_eax str_addr,str_size,result
    cmp [result],0
    je exit
    rol eax,16
    mov bx,ax
    rol eax,16
    test ax,bx
    jz exit
    clr eax
    mov byte[result],0
  exit:
    pop ebx
}

macro atoi_al str_addr,str_size,result
{
    local exit
    mov byte[result],1
    atoi_ax str_addr,str_size,result   
    jc exit           
    test ah,ah              
    jz exit
    clr eax                          
    mov byte[result],0                     
  exit:
}

macro itoa_eax_dec in_addr,in_size,out_addr,out_size,result
{ 
    local begin,write,exit
    push eax,esi,ebx,edx
    mov byte[result],0
    mov bl,11
    cmp bl,byte[in_size]
    jl exit
    mov byte[out_size],0
    mov byte[result],1
    mov byte[in_addr+11],'$'
    lea si,[in_addr+11]
    mov ebx,10
  begin:
    mov edx,0
    div ebx
    add dl,48
    dec si
    inc byte[out_size]
    mov[si],dl
    cmp eax,0
    jz write
    jmp begin
  write:
    mov word[out_addr],si
  exit:
    pop edx,ebx,esi,eax
}

macro reg_to_binary reg,size,in_addr,in_size,out_addr,result
{
    local begin,set,skeep,exit
    push ecx,edi,eax
    mov byte[result],0
    mov al,byte[in_size]
    cmp byte[size],al
    pop eax
    jl exit
    mov byte[result],1
    mov di,in_addr
    movzx cx,byte[size]
  begin:
    rol reg,1
    jc set
    mov byte[di],'0'
    jmp skeep
  set:
    mov byte[di],'1'
  skeep:
    inc di
    loop begin
    mov byte[di],'$'
    mov [out_addr], in_addr
  exit:
    pop edi,ecx
}

macro get_char 
{
  push eax
  mov ah,0 
  int 16h
  pop eax
}

macro clear_screen
{
  push eax
  mov ax,03
  int 10h
  pop eax
}

macro close_program
{
  mov eax,4c00h
  int 21h
}

macro sleep_time time
{ 
  push ecx,edx,eax
  mov cx,word[time]
  mov dx,word[time+2]
  mov ah,86h 
  int 15h
  pop eax,edx,ecx
}

macro sleep_iter iter
{
  push ecx,eax
  mov ecx,iter
  local it
  it:
  nop
  loop it
  pop eax,ecx 
}

