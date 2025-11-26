  AREA APOS_CODE, CODE, READONLY
  THUMB
  EXPORT APOS_save_regs
  EXPORT APOS_restore_regs


APOS_save_regs
  ; r0: sp
  ; move sp by 8*4 bytes (r4-r11)
  SUBS r0, r0, #32
  
  STR r4, [r0, #0]
  STR r5, [r0, #4]
  STR r6, [r0, #8]
  STR r7, [r0, #12]
 
    ; save r8-r11 (need to move to low reg first as direct LDR not supported)
  MOV r2,   r8
  STR r2,   [r0, #16]
  
  MOV r2,   r9
  STR r2,   [r0, #20]
  
  MOV r2,   r10
  STR r2,   [r0, #24]
  
  MOV r2,   r11
  STR r2,   [r0, #28]
  ; return value = new sp
  
  BX lr
  
APOS_restore_regs
  ; r0: sp

  ; restore r0-r7
  LDR r4,   [r0, #0]
  LDR r5,   [r0, #4]
  LDR r6,   [r0, #8]
  LDR r7,   [r0, #12]    
  ; restore r8-r11 (need to move to low reg first as direct LDR not supported)
  LDR     r2, [r0, #16]
  MOV     r8, r2

  LDR     r2, [r0, #20]
  MOV     r9, r2

  LDR     r2, [r0, #24]
  MOV     r10, r2

  LDR     r2, [r0, #28]
  MOV     r11, r2
  
  ; add r4-r11 to sp
  ADDS  r0, r0, #32
  ; load psp
  MSR PSP,  r0
  
  BX  lr
    
END