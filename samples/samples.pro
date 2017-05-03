
TEMPLATE = subdirs

include( ../config.pri )

contains( DEFINES, ARGS_QSTRING_BUILD ) {

SUBDIRS =

} else {

SUBDIRS = help \
	sample_with_commands

}
