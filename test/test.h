#ifndef test_h
#define test_h



class device_ {
  private:
    uint8_t buttons;
  
  public:
    device_(void);
    void begin();
    void doReport();
};

extern device_ device;

#endif // #ifndef test_h