#ifndef MICPPS_MIPSTYPES_H
#define MICPPS_MIPSTYPES_H

#include <cinttypes>

namespace mips {
	namespace instruction_types {
		struct __attribute__ ((packed)) rtype {
			uint8_t funct : 6;
			uint8_t shant : 5;
			uint8_t rd    : 5;
			uint8_t rt    : 5;
			uint8_t rs    : 5;
			uint8_t op    : 6;
		};

		struct __attribute__ ((packed)) itype {
			uint16_t imm : 16;
			uint8_t  rt  : 5;
			uint8_t  rs  : 5;
			uint8_t  op  : 6;
		};

		struct __attribute__ ((packed)) jtype {
			uint32_t target : 26;
			uint8_t  op     : 6;
		};
	}

	union instruction {
		uint32_t                 bytecode;
		instruction_types::itype itype;
		instruction_types::jtype jtype;
		instruction_types::rtype rtype;
	};
}

#endif //MICPPS_MIPSTYPES_H
