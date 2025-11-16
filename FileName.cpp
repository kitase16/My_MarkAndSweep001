#include <iostream>
#include <memory>
#include <cstdint>
#include <vector>
#include <algorithm>

#include <crtdbg.h>//vc only?

#define interface struct

struct Memory {
	typedef std::uint8_t Elem;
	void GetSelf(const std::shared_ptr<Memory>& In) {// this is must need by your hand. so first and fast.
		if (In.get() == this) {
			Self = In;
		}
	}

	Elem& operator[](std::size_t Idx){
		return Mem[Idx];
	}

	bool Resize(std::size_t N) {
		if (Mem.size() <= N) { return false; }
		Mem.resize(N);
	}

	std::size_t Size() {
		return Mem.size();
	}
	interface MemCh;
	std::shared_ptr<MemCh> MakeChile(std::size_t N) {
		std::sort(Child.begin(), Child.end(), [](auto& A, auto& B) {return A->Point() < B->Point(); });

		std::shared_ptr<MemCh> Tmp;
		std::size_t PA = Child.back()->Size();
		std::size_t PB = 0;
		std::size_t SzA = Child.back()->Size();
		for (auto& o : Child) {
			if (PB + N < PA) { break; }
			PA = o->Point();
			SzA = o->Size();
			PB = PA + SzA;
		}

		if (PB + N >= Size()) {
			return std::make_shared<MemCh>(nullptr);
		}

		auto R =  std::make_shared<MemCh>(PB, N);
		R->SetMoving(true);
		return R;
	}

	bool Sweep() {
		for (std::size_t i = 0; i < Child.size(); i++) {
			if (!Child[i]->IsMoving()) {
				Child.erase(Child.begin() + i);
				i--;
			}
		}
		return true;
	}

	bool Kill() {
		this->Mem.clear();
		this->Child.clear();
		this->Self.reset();
		return true;
	}

	struct MemCh{
	public:
		MemCh(std::size_t P_,std::size_t Sz_): P(P_),Sz(Sz_) {}

		bool IsIn(std::size_t P_,std::size_t Sz_,std::size_t Pos_) {
			if (((Sz_ - P_) * (Sz - P_)) + ((Sz - P) * (Sz - P)) <= (Sz * Sz) + (Sz_ * Sz_)) {
				return Sz_>=Pos_;
			}
			return false;
		}

		std::size_t Point() {
			return P;
		}

		std::size_t Size() {
			return Sz;
		}

		const Elem& Front() {
			return (*Mom)[P];
		}
		const Elem& Back() {
			int A = 0;
			if (Sz != 0) { A = -1; }
			return (*Mom)[P+Sz+A];
		}
		Elem& operator[](std::intmax_t Idx) {
			return (*Mom)[P + Idx];
		}

		void Kill() {
			Mom = std::shared_ptr<Memory>(nullptr);
			P = 0;
			Sz = 0;
			SetMoving(false);
			return;
		}


		~MemCh() {
			for (std::size_t i = P; i < P + Sz; i++) {//erase secret.
				(*this)[i] = Memory::Elem();
			}
			Kill();
		}

		bool IsMoving() {
			return F;
		}

		void SetMoving(bool FF) {
			F = FF;
		}

	protected:
		bool F = false;
		std::size_t P = 0;
		std::size_t Sz = 0;
		std::shared_ptr<Memory> Mom;
	};
protected:
	std::vector<Elem> Mem;
	std::vector<std::shared_ptr<MemCh>> Child;
	std::shared_ptr<Memory> Self;
};

typedef std::shared_ptr<Memory> SharedMemory;

int main() {

	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);

	SharedMemory m = std::shared_ptr<Memory>();
	m->GetSelf(m);//cross refalence. not easy destroy.
	m->Resize(1024);

	auto C = m->MakeChile(16);


	m->Kill();

	

	return 0;
}



