  AREA APOS_CODE, CODE, READONLY
  THUMB
  EXPORT APOS_save_regs
  EXPORT APOS_restore_regs


APOS_save_regs
  ; r0: sp
  
  
APOS_restore_regs
    ; r0: sp

    ; restore r0-r7
    LDR r4,   [r0, #0]
    LDR r5,   [r0, #4]
    LDR r6,   [r0, #8]
    LDR r7,   [r0, #12]    
    ; restore r8-r11 (need to move to low reg first as direct LDR not supported)
    MOV r2,   r8
    STR r2,   [r0, #16]
    
    MOV r2,   r9
    STR r2,   [r0, #20]
    
    MOV r2,   r10
    STR r2,   [r0, #24]
    
    MOV r2,   r11
    STR r2,   [r0, #28]
    
    ; add r4-r11 to sp
    ADDS  r0, r0, #32
    ; load psp
    MSR PSP,  r0
    
    BX  lr
    
END