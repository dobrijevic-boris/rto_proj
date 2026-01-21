  AREA APOS_CODE, CODE, READONLY
  THUMB
  EXPORT APOS_save_regs
  EXPORT APOS_restore_regs
  EXPORT PendSV_Handler
  IMPORT APOS_Select_next_task
 
  EXPORT SVC_Handler
  IMPORT pCurrentTask         ; Import the variable we exposed in C
    
PendSV_Handler
        MRS     r0, PSP          ; r0 = current PSP (thread context)
        BL      APOS_save_regs   ; r0 = PSP after saving r4 r11
        BL      APOS_Select_next_task   ; r0 = PSP of next task (from TCB_Tasks[state])
        BL      APOS_restore_regs; restore r4 r11 and MSR PSP
        ;EXC_Return to trigger new pc
        LDR r0, =0xFFFFFFFD       ;  restore original EXC_RETURN
        MOV lr, r0
        BX  lr                   ; return

SVC_Handler
    ; 1. Load address of pCurrentTask
    LDR     R1, =pCurrentTask
    LDR     R2, [R1]          ; R2 = pointer to TCB struct
    
    ; 2. Load pStack from TCB (Offset 16 bytes: Name(4)+Prio(4)+Base(4)+Routine(4))
    LDR     R0, [R2, #16]     ; R0 = pStack (passed as arg to restore_regs)
    
    ; 3. Call your existing restore function
    BL      APOS_restore_regs ; Restores R4-R11 and sets PSP
    
    ; 4. Return to Thread Mode (Process Stack)
    LDR     R0, =0xFFFFFFFD   
    MOV     LR, R0
    BX      LR

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

  ; restore r4-r7
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
  
  BX lr
    
  END