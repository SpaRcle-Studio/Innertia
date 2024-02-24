// 
// Created by qulop on 24.02.2024
// 
#include <Core/Transaction.h>


namespace IR_CORE_NS {
// TXInput
	TXInput::TXInput(HashPtr out, uint32_t sequence)
		: m_out(out)
		, m_sequence(sequence)
	{}

	HashPtr TXInput::GetOut() const {
		return m_out;
	}

	uint32_t TXInput::GetSequence() const {
		return m_sequence;
	}
// -----------

// TXOutput
	TXOutput::TXOutput(float value)
		: m_value(value)
	{}


	float TXOutput::GetValue() const {
		return m_value;
	}
// --------

// Transaction
	HashPtr Transaction::GetHash() const {
		return m_hash;
	}


	HashPtr Transaction::ComputeHash() {
		return nullptr;
	}

}
