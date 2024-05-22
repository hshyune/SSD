class TestShell {
public:
	void write(int address, int data) {
		/*
		3번 LBA 에 0xAAAABBBB 를 기록한다.
		ssd 에 명령어를 전달한다
		*/
	}

	int read(int address, int times) {
		//times 횟수만큼 LBA 를 읽는다.
		//ssd 에 명령어를 전달한다.
		//result.txt 에 적힌 결과를 화면에 출력한다.
		return 0;
	}

	void exit() {
		// Shell 이 종료된다
	}

	void help() {
		// 각 명령어당 사용 방법을 출력한다
	}

	void fullwrite(int data) {
		//LBA 0 번부터 99 번 까지 Write를 수행한다.
		//ssd 전체에 값이 써진다.
		//ex) fullwrite 0xABCDFFFF
		//모든 LBA에 값 0xABCDFFF 가 적힌다.
	}
	void  fullread() {
		//LBA 0 번부터 99 번 까지 Read를 수행한다.
		//ssd 전체 값을 모두 화면에 출력한다.
		//ex) fullread
		//모든 LBA의 값들이 화면에 출력된다
	}
};