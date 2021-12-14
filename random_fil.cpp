
/**  
 * Under the hood it would be looks like imul x,a; add out, 8; add x,c ; mov out-8, x;
 * instr 1,2,4 are strongly bounded. 
 * 
 * Crossiterations dependencies can be precalculated for some range, 
 * so we need arr of a pow and c scaled with prev a pows by some size
 * So in this case we would have subcycle that could be vectorized by compiler
 * 
 * n does not have to be divisible by any number, so (n mod vector size) of elems would be calculated as is
 *
 */
// void randomfill_o(uint64_t *out, size_t n, uint64_t x, uint64_t a, uint64_t c) {
// 	for(;n;n--) {
// 		x = x * a + c;
// 		*out++ = x;
// 	}
// }
void randomfill(uint64_t *out, size_t n, uint64_t x, uint64_t a, uint64_t c) {
	const size_t vectorLength = 8;

	uint64_t aRow[vectorLength];
	uint64_t cRow[vectorLength];

	aRow[0] = a;
	cRow[0] = c;

	for(size_t i = 1; i < vectorLength; ++i) {
		aRow[i] = aRow[i-1]*a;
		cRow[i] = cRow[i-1] + (aRow[i-1]*c);
	}

	const size_t alignedSize = n - (n % vectorLength);

	for(size_t i = 0; i < alignedSize; i += vectorLength) {
		for(size_t j = 0; j < vectorLength; ++j) {
			out[i+j] = x * aRow[j] + cRow[j];
		}
		x = out[i+vectorLength-1];
	}

	for(size_t i = alignedSize; i < n; ++i) {
		x = x * a + c;
		*out++ = x;
	}
}