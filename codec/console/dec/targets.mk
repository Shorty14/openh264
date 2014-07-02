H264DEC_SRCDIR=codec/console/dec
H264DEC_CPP_SRCS=\
	$(H264DEC_SRCDIR)/src/d3d9_utils.cpp\
	$(H264DEC_SRCDIR)/src/h264dec.cpp\

H264DEC_OBJS += $(H264DEC_CPP_SRCS:.cpp=.$(OBJ))

OBJS += $(H264DEC_OBJS)
$(H264DEC_SRCDIR)/%.$(OBJ): $(H264DEC_SRCDIR)/%.cpp
	$(QUIET_CXX)$(CXX) $(CFLAGS) $(CXXFLAGS) $(INCLUDES) $(H264DEC_CFLAGS) $(H264DEC_INCLUDES) -c $(CXX_O) $<

h264dec$(EXEEXT): $(H264DEC_OBJS) $(H264DEC_DEPS)
	$(QUIET_CXX)$(CXX) $(CXX_LINK_O) $(H264DEC_OBJS) $(H264DEC_LDFLAGS) $(LDFLAGS)

binaries: h264dec$(EXEEXT)
BINARIES += h264dec$(EXEEXT)
