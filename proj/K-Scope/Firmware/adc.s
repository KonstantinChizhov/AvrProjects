.global	__vector_14
	.type	__vector_14, @function
__vector_14:
.LFB10:
.LM55:
/* prologue: frame size=0 */
	push __zero_reg__
	push __tmp_reg__
	in __tmp_reg__,__SREG__
	push __tmp_reg__
	clr __zero_reg__
	push r18
	push r19
	push r24
	push r25
	push r30
	push r31
/* prologue end (size=11) */
.LM56:
/* #APP */
	sei
.LM57:
/* #NOAPP */
	lds r24,buffer_pos
	lds r25,(buffer_pos)+1
	subi r24,lo8(700)
	sbci r25,hi8(700)
	brlo .L51
.LBB20:
.LBB21:
.LM58:
	out 39-0x20,__zero_reg__
.LM59:
	out 38-0x20,__zero_reg__
.LBE21:
.LBE20:
.LM60:
	sts (buffer_pos)+1,__zero_reg__
	sts buffer_pos,__zero_reg__
.LM61:
	ldi r24,lo8(1)
	sts data_ready,r24
.L51:
.LM62:
	lds r24,buffer_pos
	lds r25,(buffer_pos)+1
	in r18,36-0x20
	in r19,(36)+1-0x20
	movw r30,r24
	subi r30,lo8(-(adc_buffer))
	sbci r31,hi8(-(adc_buffer))
	lsr r19
	ror r18
	lsr r19
	ror r18
	st Z,r18
	adiw r24,1
	sts (buffer_pos)+1,r25
	sts buffer_pos,r24
/* epilogue: frame size=0 */
	pop r31
	pop r30
	pop r25
	pop r24
	pop r19
	pop r18
	pop __tmp_reg__
	out __SREG__,__tmp_reg__
	pop __tmp_reg__
	pop __zero_reg__
	reti
