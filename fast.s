	# void fast_set(Digit *y, long n, Digit x)
	# y[i] = x for i in [0,n)

	.align	8
	.globl	fast_set
fast_set:
	lea	(%rdi,%rsi,8),%rdi
	neg	%rsi
1:	mov	%rdx,(%rdi,%rsi,8)
	inc	%rsi
	jnz	1b
	ret
	

	# void fast_copy(Digit *w, Digit *x, long s)
	# w[s] = x[s]
	
	.align	8
	.globl fast_copy
fast_copy:
	lea	(%rdi,%rdx,8),%rdi
	lea	(%rsi,%rdx,8),%rsi
	neg	%rdx
1:	mov	(%rsi,%rdx,8),%rax
	mov	%rax,(%rdi,%rdx,8)
	inc	%rdx
	jnz	1b
	ret

	# long fast_is_zero(Digit *x, long n)

	.align	8
	.globl	fast_is_zero
fast_is_zero:
	lea	(%rdi,%rsi,8),%rdi
	neg	%rsi
	xor	%rax,%rax
1:	cmp	(%rdi,%rsi,8),%rax
	jne	2f
	inc	%rsi
	jnz	1b
	inc	%rax
2:	ret

	# long fast_compare(Digit *x, Digit *y, long n)

	.align	8
	.globl	fast_compare
fast_compare:
	mov	-8(%rdi,%rdx,8),%rax
	sub	-8(%rsi,%rdx,8),%rax
	ja	1f
	jb	2f
	dec	%rdx
	jnz	fast_compare
	ret
1:	mov	$1,%rax
	ret
2:	mov	$-1,%rax
	ret


	# Digit fast_add(Digit *z, Digit *x, Digit *y, long n, Digit c)
	# z[n] = x[n] + y[n] + c

	.align	8
	.globl	fast_add
fast_add:
	lea	(%rdi,%rcx,8),%rdi
	lea	(%rsi,%rcx,8),%rsi
	lea	(%rdx,%rcx,8),%rdx
	neg	%rcx
	bt	$0,%r8w
1:	mov	(%rsi,%rcx,8),%rax
	adc	(%rdx,%rcx,8),%rax
	mov	%rax,(%rdi,%rcx,8)
	inc	%rcx
	jnz	1b
	setc	%al
	movzbq	%al,%rax
	ret

	.align	8
	.globl	fast_add_carry

	# void fast_add_carry(Digit *y, Digit *x, long n, Digit c)
	# y[n+] += x[n] + c

fast_add_carry:	
	lea	(%rdi,%rdx,8),%rdi
	lea	(%rsi,%rdx,8),%rsi
	neg	%rdx
	bt	$0,%cx
1:	mov	(%rsi,%rdx,8),%rax
	adc	%rax,(%rdi,%rdx,8)
	inc 	%rdx
	jnz	1b
	jnc	3f
2:	adcq	$0,(%rdi,%rdx,8)
	inc	%rdx
	jc	2b
3:	ret
	
	# Digit fast_carry(Digit *y, Digit *x, long n)
	# y[n] = x[y] + 1
	.align	8
	.globl	fast_carry
fast_carry:
	lea	(%rdi,%rdx,8),%rdi
	lea	(%rsi,%rdx,8),%rsi
	neg	%rdx
	xor	%rax,%rax
	stc
1:	mov	(%rsi,%rdx,8),%rcx
	adc	%rax,%rcx
	mov	%rcx,(%rdi,%rdx,8)
	jnc	2f
	inc	%rdx
	jnz	1b
	inc	%rax
	jmp	4f
2:	cmp	%rsi,%rdi
	je	4f
	inc	%rdx
	jz	4f
3:	mov	(%rsi,%rdx,8),%rcx
	mov	%rcx,(%rdi,%rdx,8)
	inc	%rdx
	jnz	3b
4:	ret

	# Digit fast_subtract(Digit *z, Digit *x, Digit *y, long n, Digit c)
	# z[n] = x[n] - y[n] - c
	.align 8
	.globl	fast_subtract
fast_subtract:
	lea	(%rdi,%rcx,8),%rdi
	lea	(%rsi,%rcx,8),%rsi
	lea	(%rdx,%rcx,8),%rdx
	neg	%rcx
	bt	$0,%r8w
1:	mov	(%rsi,%rcx,8),%rax
	sbb	(%rdx,%rcx,8),%rax
	mov	%rax,(%rdi,%rcx,8)
	inc	%rcx
	jnz	1b
	setc	%al
	movzbq	%al,%rax
	neg	%rax
	ret

	.align	8
	.globl	fast_subtract_borrow
	# void fast_subtract_borrow(Digit *y, Digit *x, long n, Digit c) {
	# y[n+] -= x[n] + c

fast_subtract_borrow:
	lea	(%rdi,%rdx,8),%rdi
	lea	(%rsi,%rdx,8),%rsi
	neg	%rdx
	bt	$0,%cx
1:	mov	(%rsi,%rdx,8),%rax
	sbb	%rax,(%rdi,%rdx,8)
	inc	%rdx
	jnz	1b
	jnc	3f
2:	sbbq	$0,(%rdi,%rdx,8)
	inc	%rdx
	jc	2b
3:	ret
	
	# Digit fast_borrow(Digit *y, Digit *x, long n)
	# y[n] = x[n] - 1
	.align	8
	.globl	fast_borrow
fast_borrow:
	lea	(%rdi,%rdx,8),%rdi
	lea	(%rsi,%rdx,8),%rsi
	neg	%rdx
	xor	%rax,%rax
	stc
1:	mov	(%rsi,%rdx,8),%rcx
	sbb	%rax,%rcx
	mov	%rcx,(%rdi,%rdx,8)
	jnc	2f
	inc	%rdx
	jnz	1b
	inc 	%rax
	neg	%rax
	jmp	4f
2:	cmp	%rsi,%rdi
	je	4f
	inc	%rdx
	jz	4f
3:	mov	(%rsi,%rdx,8),%rcx
	mov	%rcx,(%rdi,%rdx,8)
	inc	%rdx
	jnz	3b
4:	ret
	
	# Digit fast_multiply(Digit *z, Digit *x, long n, Digit y, Digit c)
	# z[n] = x[n]*y + c

	.align	8
	.globl	fast_multiply
fast_multiply:
	xchg	%rcx,%rdx
	mov	%r8,%rax
	lea	(%rdi,%rcx,8),%rdi
	lea	(%rsi,%rcx,8),%rsi
	neg	%rcx
	clc
1:	mulx	(%rsi,%rcx,8),%r8,%r9
	adc	%rax,%r8
	mov	%r8,(%rdi,%rcx,8)
	mov	%r9,%rax
	inc	%rcx
	jnz	1b
	adc	$0,%rax
	ret

	# Digit fast_divide(Digit *z, Digit *x, long n, Digit y)
	# z[n] = x[n]/y   

	.align	8
	.globl	fast_divide
fast_divide:
	mov	%rdx,%r8
	xor	%rdx,%rdx
1:	mov	-8(%rsi,%r8,8),%rax
	div	%rcx
	mov	%rax,-8(%rdi,%r8,8)
	dec	%r8
	jnz	1b
	mov	%rdx,%rax
	ret

	# Digit fast_inverse(Digit *z, Digit x, Digit y, long, n)
	# Z[n] = x/y   -- x should be less than y, y should not be 0

	.align	8
	.globl	fast_inverse
fast_inverse:
	mov	%rdx,%r8
	xor	%rdx,%rdx
	mov	%rsi,%rax
1:	div	%r8
	mov	%rax,-8(%rdi,%rcx,8)
	xor	%rax,%rax
	dec	%rcx
	jnz	1b
	mov	%rdx,%rax
	ret
	
