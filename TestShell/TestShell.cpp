class TestShell {
public:
	void write(int address, int data) {
		/*
		3�� LBA �� 0xAAAABBBB �� ����Ѵ�.
		ssd �� ��ɾ �����Ѵ�
		*/
	}

	int read(int address) {
		//3�� LBA �� �д´�.
		//ssd �� ��ɾ �����Ѵ�.
		//result.txt �� ���� ����� ȭ�鿡 ����Ѵ�.
		return 0;
	}

	void exit() {
		// Shell �� ����ȴ�
	}

	void help() {
		// �� ��ɾ�� ��� ����� ����Ѵ�
	}

	void fullwrite(int data) {
		//LBA 0 ������ 99 �� ���� Write�� �����Ѵ�.
		//ssd ��ü�� ���� ������.
		//ex) fullwrite 0xABCDFFFF
		//��� LBA�� �� 0xABCDFFF �� ������.
	}
	void  fullread() {
		//LBA 0 ������ 99 �� ���� Read�� �����Ѵ�.
		//ssd ��ü ���� ��� ȭ�鿡 ����Ѵ�.
		//ex) fullread
		//��� LBA�� ������ ȭ�鿡 ��µȴ�
	}
};