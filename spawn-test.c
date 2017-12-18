#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <uv.h>

uv_loop_t *loop;
uv_process_t child_req;
uv_process_options_t options;
uv_pipe_t apipe;

void on_child_exit(uv_process_t* process,
    int64_t exit_status,
    int term_signal) {
  fprintf(stderr, "Process exited with status %lli, signal %d\n", exit_status, term_signal);
  uv_close((uv_handle_t*) process, NULL);
  uv_close((uv_handle_t*) &apipe, NULL);
}

static void alloc_buffer(uv_handle_t* handle,
    size_t suggested_size,
    uv_buf_t* buf) {
  (void)handle;
  printf("alloc_buffer called, requesting a %li byte buffer\n", suggested_size);
  buf->base = (char*)malloc(suggested_size);
  buf->len = suggested_size;
}

static void read_apipe(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf) {
  (void)stream;
  if (nread > 0) {
    /* printf("read %li bytes in a %li byte buffer\n", nread, buf->len); */
    buf->base[nread] = '\0'; // turn it into a cstring
    printf("read: |%s|", buf->base);
  }
}


int main(int argc, char *argv[]) {
  (void)argc;
  (void)argv;
  printf("spawn_test\n");
  loop = uv_default_loop();

  char* args[3];
  args[0] = "./dummy";
  args[1] = NULL;
  args[2] = NULL;

  uv_pipe_init(loop, &apipe, 0);
  //uv_pipe_open(&apipe, 0);

  options.stdio_count = 3;
  uv_stdio_container_t child_stdio[3];
  /* child_stdio[0].flags = UV_IGNORE; */
  /* child_stdio[1].flags = UV_IGNORE; */
  /* child_stdio[2].flags = UV_INHERIT_FD; */
  /* child_stdio[2].data.fd = 2; */
  child_stdio[0].flags = UV_IGNORE;
  child_stdio[1].flags = UV_CREATE_PIPE | UV_WRITABLE_PIPE;
  child_stdio[1].data.stream = (uv_stream_t *) &apipe;
  child_stdio[2].flags = UV_IGNORE;
  options.stdio = child_stdio;

  options.exit_cb = on_child_exit;
  options.file = args[0];
  options.args = args;

  int err = uv_spawn(loop, &child_req, &options);
  if (err < 0) {
    fprintf(stderr, "spawn error: %s\n", uv_strerror(err));
    return 1;
  }

  uv_read_start((uv_stream_t*)&apipe, alloc_buffer, read_apipe);

  return uv_run(loop, UV_RUN_DEFAULT);
}
