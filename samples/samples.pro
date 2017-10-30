
TEMPLATE = subdirs

include( ../config.pri )

contains( DEFINES, ARGS_QSTRING_BUILD ) {

SUBDIRS = qt_help \
	api

} else {

SUBDIRS = help \
	sample_with_commands \
	api

}
