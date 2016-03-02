#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "dmg.h"
#include "filevault.h"
#include "hfsplus.h"
#include "hfslib.h"

char endianness;

void TestByteOrder() {
	short int word = 0x0001;
	char *byte = (char *) &word;
	endianness = byte[0] ? IS_LITTLE_ENDIAN : IS_BIG_ENDIAN;
}

int main(int argc, char* argv[]) {
	TestByteOrder();

	AbstractFile* in = createAbstractFileFromFile(stdin);
	AbstractFile* out = createAbstractFileFromFile(tmpfile());

	extractDmg(in, out, -1);

	Volume* volume = openVolume(IOFuncFromAbstractFile(out));

	HFSPlusCatalogRecord* record = getRecordFromPath("/", volume, NULL, NULL);

	extractAllInFolder(((HFSPlusCatalogFolder*)record)->folderID, volume);

	return 0;
}