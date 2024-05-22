
class IIoInterface {
	virtual int read() = 0;
	virtual void write() = 0;
};

class Nand : public IIoInterface {
	// IIoInterface을(를) 통해 상속됨
	int read() override
	{
		return 0;
	}
	void write() override
	{
	}
};

class SSD {
public:
	SSD(IIoInterface* nand) {
		this->nand = nand;
	}
	int read(int address) {
		return 0;
	}
	void write(int address, int data) {
		return;
	}
private:
	IIoInterface* nand;
};