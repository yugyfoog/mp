	# void multiply_schoolboy(Digit *z, Digit *x, long m, Digit *y, long n)
	# z[m+n] = x[m]*y[n]

	.globl	multiply_schoolboy
multiply_schoolboy:
	push	%r12
	push	%r13
	xor	%r9,%r9
	lea	(%rdi,%r8,8),%rdi
	lea	(%rsi,%rdx,8),%rsi
	lea	(%rcx,%r8,8),%rcx
	neg	%r8
	mov	%r8,%r10
1:	mov	%r9,(%rdi,%r10,8)
	inc	%r10
	jnz	1b
	lea	(%rdi,%rdx,8),%rdi
	mov	%rdx,%r11
	neg	%r11
2:	mov	%r8,%r12
	add	%r11,%r12
	xor	%r13,%r13
	mov	%r8,%r10
1:	mov	(%rsi,%r11,8),%rax
	mov	(%rcx,%r10,8),%rdx
	mul	%rdx
	add	(%rdi,%r12,8),%rax
	adc	%r9,%rdx
	add	%r13,%rax
	adc	%r9,%rdx
	mov	%rax,(%rdi,%r12,8)
	mov	%rdx,%r13
	inc	%r12
	inc	%r10
	jnz	1b
	mov	%r13,(%rdi,%r12,8)
	inc	%r11
	jnz	2b
	pop	%r13
	pop	%r12
	ret

	# divide_schoolboy(Digit *q, Digit *u, Digit *v, long m, long n)
	# u[m+n] should be zero
	# q[m+1] = u[m+n] / v[n]
	# u[n] = u[m+n] % v[n]
	# v[n] = junk
	
	.align	8
	.globl	divide_schoolboy
divide_schoolboy:
	push	%rbx
	push	%r12
	push	%r13
	push	%r14
	push	%r15
	push	%rbp
	mov	%rdi,%rbx
	mov	%rsi,%r12
	mov	%rdx,%r13
	mov	%rcx,%r14
	mov	%r8,%r15
	mov	$1,%rbp
	mov	$1,%rdx
	xor	%rax,%rax
	mov	-8(%r13,%r15,8),%rsi
	inc	%rsi
	jz	1f
	div	%rsi
	mov	%rax,%rbp
	cmp	$1,%rbp
	je	1f
	xor	%r8,%r8
	mov	%rbp,%rcx
	lea	1(%r14,%r15),%rdx
	mov	%r12,%rsi
	mov	%r12,%rdi
	call	fast_multiply
	xor	%r8,%r8
	mov	%rbp,%rcx
	mov	%r15,%rdx
	mov	%r13,%rsi
	mov	%r13,%rdi
	call	fast_multiply
1:	
2:
	mov	-8(%r13,%r15,8),%rcx
	lea	(%r14,%r15),%rsi
	xor	%rdx,%rdx
	mov	(%r12,%rsi,8),%rax
	div	%rcx
	mov	%rax,%rdi
	mov	-8(%r12,%rsi,8),%rax
	div	%rcx
	mov	%rax,%r8
	mov	%rdx,%r9
3:	
	test	%rdi,%rdi
	jnz	4f

	mov	-16(%r13,%r15,8),%rax
	mul	%r8
	cmp	%r9,%rdx
	ja	4f
	jb	5f
	cmp	-16(%r12,%rsi,8),%rax
	jbe	5f
4:
	sub	$1,%r8
	sbb	$0,%rdi
	add	-8(%r13,%r15,8),%r9
	jnc	3b
5:
	mov	%r8,(%rbx,%r14,8)
	test	%r8,%r8
	jz	6f
	mov	%r15,%rcx
	mov	%r13,%rdx
	lea	(%r12,%r14,8),%rsi
	mov	%rsi,%rdi
	call	multiply_subtract
	test	%rax,%rax
	jz	6f
	decq	(%rbx,%r14,8)
	mov	%r15,%rcx
	mov	%r13,%rdx
	lea	(%r12,%r14,8),%rsi
	mov	%rsi,%rdi
	call	add_back
6:
	sub	$1,%r14
	jnc	2b
	cmp	$1,%rbp
	je	7f
	mov	%rbp,%rcx
	mov	%r15,%rdx
	mov	%r12,%rsi
	mov	%r12,%rdi
	call	fast_divide
7:	
	pop	%rbp
	pop	%r15
	pop	%r14
	pop	%r13
	pop	%r12
	pop	%rbx
	
	ret
	
	# multiply_subtract(Digit *s, Digit *u, Digit *v, long n, Digit w)
	#  s[n+1] = u[n+1] - w*v[n]
	.align	8
multiply_subtract:
	xchg	%rdx,%r8
	lea	(%rdi,%rcx,8),%rdi
	lea	(%rsi,%rcx,8),%rsi
	lea	(%r8,%rcx,8),%r8
	neg	%rcx
	xor	%r9,%r9
	xor	%ax,%ax
1:
	mulx	(%r8,%rcx,8),%r10,%r11
	sahf
	adc	%r9,%r10
	lahf
	xchg	%ah,%al
	mov	%r11,%r9
	mov	(%rsi,%rcx,8),%r11
	sahf
	sbb	%r10,%r11
	lahf
	xchg	%ah,%al
	mov	%r11,(%rdi,%rcx,8)
	inc	%rcx
	jnz	1b
	sahf
	adc	$0,%r9
	xchg	%ah,%al
	mov	(%rsi,%rcx,8),%r11
	sahf
	sbb	%r9,%r11
	mov	%r11,(%rdi,%rcx,8)
	setc	%al
	movzbq	%al,%rax
	neg	%rax
	ret
	
	# add_back(Digit *w, Digit *u, Digit *v, long n)
	# w[n+1] = u[n+1] + v[n]

	.align	8
add_back:
	lea	(%rdi,%rcx,8),%rdi
	lea	(%rsi,%rcx,8),%rsi
	lea	(%rdx,%rcx,8),%rdx
	neg	%rcx
	clc
1:	mov	(%rsi,%rcx,8),%rax
	adc	(%rdx,%rcx,8),%rax
	mov	%rax,(%rdi,%rcx,8)
	inc	%rcx
	jnz	1b
	mov	(%rsi),%rax
	adc	$0,%rax
	mov	%rax,(%rdi)
	ret
	

