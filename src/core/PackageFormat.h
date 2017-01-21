#pragma once

#include <cstdint>
#include <vector>

class Package;

/**
 * @brief Struct that specifies the file format of asset packages
 *
 * @note Every project must set the PROJ_ID to the appropriate 5-character
 *       identification string (see PackageFormat.cpp)
 */
struct PackageFormat
{
public:
	enum class DataType_t : uint8_t { TEXTURE, MODEL };

	/*
	      0x40       0x20       0x10      0x8       0x4       0x2       0x1
	  +-----------------------------------------------------------------------+
	  |          |          |         |         |  wrap_v |  wrap_h | hasAlpha|
	  +-----------------------------------------------------------------------+
	*/
	struct TextureHeader
	{
		uint8_t Flags;

		enum
		{
			HAS_ALPHA = 0x1,
			WRAP_H = 0x2,
			WRAP_V = 0x4
		};
	};

private:
#ifdef PACKAGE_DEBUG
public:
#endif
	friend class Package;

	static const uint16_t CURRENT_VERSION = 0;
	static const uint16_t MIN_READER_VERSION = 0;

	enum class CompressionType_t : uint8_t { NO_COMPRESSION };
	enum class ChecksumType_t : uint8_t { NO_CHECKSUM };
	enum class EncryptionType_t : uint8_t { NO_ENCRYPTION };

	static const char ID[3];
	typedef char projID_t[5];
	static const projID_t PROJ_ID;

	struct Header
	{
		char ID[3];
		projID_t ProjID;

		EncryptionType_t EncryptionType;
		uint8_t Reserved[3];

		uint16_t FormatVersion;
		uint16_t MinReaderVersion;

		uint16_t ItemCount;
		CompressionType_t CompressionType;
		ChecksumType_t ChecksumType;

		uint32_t TablePosition;

		// Future versions can add info here
	};

	typedef std::pair<uint32_t, uint32_t> TableElement;

	struct Table
	{
		// Array of hash values to data positions
		TableElement HashToPos[1];
	};

	/*
	 * Every item in the package looks like this:
	 *  +-----------------+
	 *  |   Data Header   |
	 *  +-----------------+
	 *  |    Item Name    |
	 *  +-----------------+
	 *  |    Sub-Header   |
	 *  +-----------------+
	 *  |      Data       |
	 *  |      ....       |
	 *  +-----------------+
	 */
	struct DataHeader
	{
		uint16_t HeaderSize; // Size of sub-header (from right after DataSize)
		uint8_t NameLength;  // Length of filename (cannot be longer than 255)
		DataType_t DataType;

		uint32_t DataSize; // Note: Data starts right after secondary header ends
	};

	Header header;
	Table table;
};

