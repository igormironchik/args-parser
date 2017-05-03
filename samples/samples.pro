
TEMPLATE = subdirs

include( ../config.pri )

contains( DEFINES, ARGS_QSTRING_BUILD ) {

SUBDIRS = qt_help

} else {

SUBDIRS = help \
	sample_with_commands

}
