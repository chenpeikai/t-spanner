#define STATUS_SUCCESS  0x000

#define STATUS_ERROR_IO 0xF01
#define STATUS_ERROR_PARSING_NUMBER 0xF02
#define STATUS_ERROR_PARSING_INCOMPLETE 0xF03
#define STATUS_ERROR_PARSING_EXISTANT_EDGE 0xF04
#define STATUS_ERROR_GPU_INITIALIZATION 0xF05
#define STATUS_ERROR_GPU_NO_DEVICE_FOUND 0xF06
#define STATUS_ERROR_GPU_NOT_COMPATIBLE 0xF07
#define STATUS_ERROR_KERNEL_COMPILATION 0xF08
#define STATUS_ERROR_GRAPH_ALLOC 0xF09

#define STATUS_NO_PATH 0xA01


__attribute__((used))
static const char* error_to_str(int error)
{
  switch (error)
  {
#define CASE_ERROR(Val) case Val: return #Val
    CASE_ERROR(STATUS_SUCCESS);
    CASE_ERROR(STATUS_ERROR_IO);
    CASE_ERROR(STATUS_ERROR_PARSING_NUMBER);
    CASE_ERROR(STATUS_ERROR_PARSING_INCOMPLETE);
    CASE_ERROR(STATUS_ERROR_PARSING_EXISTANT_EDGE);
    CASE_ERROR(STATUS_ERROR_GRAPH_ALLOC);
    CASE_ERROR(STATUS_ERROR_GPU_INITIALIZATION);
    CASE_ERROR(STATUS_ERROR_GPU_NO_DEVICE_FOUND);
    CASE_ERROR(STATUS_ERROR_KERNEL_COMPILATION);
    default:
      return "UNKNOWN";
#undef CASE_ERROR
  }
}
