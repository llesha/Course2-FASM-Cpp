; Variant 13
; by Aleksey Kononov
; Task:
; Write a program which will:
; 1) get an input of array size
; 2) get an input of array A
; 3) generate array A from input
; 4) find minumum element in A
; 5) generate array B from A without all minimal elements
; 6) output arrays A and B

format PE console
entry start

include 'win32a.inc'

section '.data' data readable writable

        strVecSize   db 'size of vector? ', 0
        strIncorSize db 'Incorrect size of vector', 10, 0
        strVecElemI  db '[%d]? ', 0
        strScanInt   db '%d', 0
        strMinValue  db 'Min = %d', 10, 0
        strBValue  db 'bSize = %d', 10, 0
        strVecElemOut  db '[%d] = %d', 10, 0
        strDeb         db 'in Main', 10, 0
        strA          db 'A:', 10, 0
        strB          db 'B:', 10, 0
        strN          db ' ', 10, 0

        vec_size     dd 0
        vecBs        dd 0
        sum          dd 0
        i            dd ?
        j            dd 0
        tmp          dd ?
        tmpB         dd ?
        vec          rd 100
        vecB         rd 100
        mi           dd 2147483647
        re           dd 0
section '.code' code readable executable

start:
       call InputSize

       call GenerateA
       invoke printf, strA
       add esp, 4

       call GenerateBandOutA
       invoke printf, strB
       add esp, 4
       call OutB

       invoke printf, strN
       invoke printf, strMinValue, [mi]
       add esp, 8

finish:
       call [getch]

       push 0
       call [ExitProcess]

getOut:
        dec [vecBs]
        mov ecx, [i]
        inc ecx
        mov ebx, [tmp]
        add ebx, 4
        jmp putVecLoop

getMin:
        call Min
        jmp getVecLoop
generateMin:                    ; first element of A is minimum now
        mov [mi], ecx

        mov ecx, [i]
        inc ecx                 ; ecx++
        mov ebx, [tmp]
        add ebx, 4

        jmp getVecLoop
                                ; procedures

proc InputSize
        invoke printf, strVecSize
        add esp, 4

        push vec_size
        push strScanInt
        call [scanf]
        add esp, 8

        mov eax, [vec_size]
        mov [vecBs], eax
        cmp eax, 0
        jg  inputOut
                                ; fail size
        invoke printf, strIncorSize
        call [getch]
        push 0
        call [ExitProcess]
    inputOut:
        ret
endp

proc Min                        ; find minimum
        mov [mi], eax

        mov ecx, [i]
        inc ecx                 ; ecx++
        mov ebx, [tmp]
        add ebx, 4
        ret
endp

proc GenerateBandOutA
    putVector:                  ; here generate B
        xor ecx, ecx            ; ecx = 0
        mov ebx, vec            ; ebx = &vec

        mov edi, vecB           ; for B
        xor esi, esi

    putVecLoop:
        mov [tmp], ebx
        cmp ecx, [vec_size]
        jge Bout                ; to end of loop
        mov [i], ecx

        mov [j], esi
        mov [tmpB], edi

                                ; output element
        push dword [ebx]
        push ecx
        push strVecElemOut
        call [printf]
        add esp, 12

        mov ebp, dword [ebx]
        cmp [mi], ebp
        je getOut

        mov [vecB + esi*4], ebp ; for B
        mov esi, [j]
        inc esi
        mov edi, [tmpB]
        add edi, 4

        mov ecx, [i]
        inc ecx
        mov ebx, [tmp]
        add ebx, 4

        jmp putVecLoop
    Bout:
        ret
endp

proc OutB
    putB:
        mov ebx, vecB           ; for B
        xor ecx, ecx
    putBLoop:
        mov [tmpB], ebx
        cmp ecx, [vecBs]
        jge Bout
        mov [j], ecx


        push dword [ebx]
        push ecx
        push strVecElemOut
        call [printf]
        add esp, 12

        mov ecx, [j]
        inc ecx
        mov ebx, [tmpB]
        add ebx, 4

        jmp putBLoop

    finalOut:
    ret
endp

proc GenerateA
     getVector:
        xor ecx, ecx            ; ecx = 0
        mov ebx, vec            ; ebx = &vec
    getVecLoop:                 ; here find min element
        mov [tmp], ebx
        cmp ecx, [vec_size]
        jge Aout                ; to end of loop

                                ; input element
        mov [i], ecx
        push ecx
        push strVecElemI
        call [printf]
        add esp, 8

        push ebx
        push strScanInt
        call [scanf]
        add esp, 8

        cmp ecx, 1
        je generateMin

        mov eax, [ebx]          ; new minimum
        cmp [mi], eax
        jg getMin

        mov ecx, [i]
        inc ecx                 ; ecx++
        mov ebx, [tmp]
        add ebx, 4              ; to next element
        jmp getVecLoop
    Aout:
        ret
endp
;-------------------------------third act - including HeapApi--------------------------
                                                 
section '.idata' import data readable
    library kernel, 'kernel32.dll',\
            msvcrt, 'msvcrt.dll',\
            user32,'USER32.DLL'

include 'api\user32.inc'
include 'api\kernel32.inc'
    import kernel,\
           ExitProcess, 'ExitProcess',\
           HeapCreate,'HeapCreate',\
           HeapAlloc,'HeapAlloc'
  include 'api\kernel32.inc'
    import msvcrt,\
           printf, 'printf',\
           scanf, 'scanf',\
           getch, '_getch'