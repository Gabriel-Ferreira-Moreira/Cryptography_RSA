// shift left by less than a word
static void modshl_PM266_ct(unsigned int n, spint *a) {
  int i;
  a[9] = ((a[9] << n)) | (a[8] >> (27u - n));
  for (i = 8; i > 0; i--) {
    a[i] = ((a[i] << n) & (spint)0x7ffffff) | (a[i - 1] >> (27u - n));
  }
  a[0] = (a[0] << n) & (spint)0x7ffffff;
}

// shift right by less than a word. Return shifted out part
static int modshr_PM266_ct(unsigned int n, spint *a) {
  int i;
  spint r = a[0] & (((spint)1 << n) - (spint)1);
  for (i = 0; i < 9; i++) {
    a[i] = (a[i] >> n) | ((a[i + 1] << (27u - n)) & (spint)0x7ffffff);
  }
  a[9] = a[9] >> n;
  return r;
}

// export to byte array
static void modexp_PM266_ct(const spint *a, char *b) {
  int i;
  spint c[10];
  redc_PM266_ct(a, c);
  for (i = 33; i >= 0; i--) {
    b[i] = c[0] & (spint)0xff;
    (void)modshr_PM266_ct(8, c);
  }
}
