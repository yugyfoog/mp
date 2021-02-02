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

	# long division
	# algorithm D from Knuth, AOCP v3 4.3.1 (third edition)

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
	mov	%rdi,%rbx		# Q
	mov	%rsi,%r12		# U
	mov	%rdx,%r13		# V
	mov	%rcx,%r14		# M
	mov	%r8,%r15		# N
	
	# D1
	
	mov	$1,%rbp			# D = 1
	mov	$1,%rdx          	# %rdx = 1
	xor	%rax,%rax		# %rax = 0
	mov	-8(%r13,%r15,8),%rsi	# %rsi = V[N-1] -- should not be 0 (bad things will happen)
	inc	%rsi			# %rsi++
	jz	1f			# if %rsi is zero (overflow) then skip division and multiplication (D==1)
	div	%rsi			# %rax = 2^64 div (v[n-1] + 1)) 
	mov	%rax,%rbp		# D = %rax
	cmp	$1,%rbp		 	# if D(%rbp) == 1
	je	1f			#     skip multiplication
	xor	%r8,%r8		 	#     fast_multiply(u, u, m+n+1, d, 0)
	mov	%rbp,%rcx
	lea	1(%r14,%r15),%rdx
	mov	%r12,%rsi
	mov	%r12,%rdi
	call	fast_multiply
	xor	%r8,%r8 		#    fast_multiply(v, v, n, d, 0)
	mov	%rbp,%rcx
	mov	%r15,%rdx
	mov	%r13,%rsi
	mov	%r13,%rdi
	call	fast_multiply
	
	# D2
1:	
	# the variable M(%r14) is now called J(%r14)
2:
	# D3

	mov	-8(%r13,%r15,8),%rcx	# %rcx = V[N-1]
	lea	(%r14,%r15),%rsi	# %rsi = J+N
	xor	%rdx,%rdx		# %rdx = 0
	mov	(%r12,%rsi,8),%rax	# %rax = U[J+N]
	div	%rcx			# U[J+N] div V[N-1]
	mov	%rax,%rdi	 	# %rdi = %rax, high word for QHAT (either 1 or 0)
	mov	-8(%r12,%rsi,8),%rax	# %rax = U[J+N-1]	
	div	%rcx			# %rdx:U[J+N-1] div V[N-1]
	mov	%rax,%r8                # %r8 = %rax, [%rdi:%r8] is now double word qhat
	mov	%rdx,%r9		# %r9 = %rdx,  %r9 is now rhat
3:	
	test	%rdi,%rdi	 	# if qhat >= 2^64, i.e., %rdi != 0
	jnz	4f		 	#    do correction

	mov	-16(%r13,%r15,8),%rax	# %rax = V[N-2]
	mul	%r8			# %rdx:%rax = V[N-2]*QHAT
	cmp	%r9,%rdx		# if %rdx:%rax > rhat:U[J+N-2]
	ja	4f			#    do correction
	jb	5f			# else
	cmp	-16(%r12,%rsi,8),%rax	#    break from loop
	jbe	5f
4:					# correction:
	sub	$1,%r8			#     qhat--
	sbb	$0,%rdi			#     don't forget high word of qhat
	add	-8(%r13,%r15,8),%r9	#     rhat += v[n-1]
	jnc	3b			#     if (rhat >= 2^64) break

	# D4
5:
	mov	%r8,(%rbx,%r14,8)	# q[j] = qhat, save qhat now so we can use that register
	test	%r8,%r8			# if (qhat != 0)
	jz	6f
	mov	%r15,%rcx		#     multiply_subtract(u+j, u+j, v, n, qhat)
	mov	%r13,%rdx
	lea	(%r12,%r14,8),%rsi
	mov	%rsi,%rdi
	call	multiply_subtract

	# D5

	test	%rax,%rax		# if there was a borrow returned from multiply_subtract
	jz	6f
	
	decq	(%rbx,%r14,8)		#     q[j]--
	mov	%r15,%rcx		#     add_back(u+j, u+j, v, n)
	mov	%r13,%rdx
	lea	(%r12,%r14,8),%rsi
	mov	%rsi,%rdi
	call	add_back
6:	

	# D7

	sub	$1,%r14			# dec doesn't set the carry bit
	jnc	2b

	# D8
	
	cmp	$1,%rbp		# if (d > 1)
	je	7f
	mov	%rbp,%rcx		#     fast_divide(u, u, n, d)
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
	xchg	%rdx,%r8	# 
				# S = %rdi
				# U = %rsi
				# V = %r8
				# N = %rcx
				# W = %rdx
	lea	(%rdi,%rcx,8),%rdi
	lea	(%rsi,%rcx,8),%rsi
	lea	(%r8,%rcx,8),%r8
	neg	%rcx
	xor	%r9,%r9
	xor	%ax,%ax
1:
	# multiply
	
	mulx	(%r8,%rcx,8),%r10,%r11	# T1:T0 = v[n]*w
	sahf			# restore mflag
	adc	%r9,%r10
	lahf			# save mflag
	xchg	%ah,%al

	mov	%r11,%r9

	# subtract

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
	

