
php 实现的map


struct _zend_array {
	zend_refcounted_h gc;
	union {
		struct {
			ZEND_ENDIAN_LOHI_4(
				uint8_t    flags,
				uint8_t    _unused,
				uint8_t    nIteratorsCount,
				uint8_t    _unused2)
		} v;
		uint32_t flags;
	} u;
	uint32_t          nTableMask;
	union {
		uint32_t     *arHash;   /* hash table (allocated above this pointer) */
		Bucket       *arData;   /* array of hash buckets */
		zval         *arPacked; /* packed array of zvals */
	};
	uint32_t          nNumUsed;
	uint32_t          nNumOfElements;
	uint32_t          nTableSize;
	uint32_t          nInternalPointer;
	zend_long         nNextFreeElement;
	dtor_func_t       pDestructor;
};