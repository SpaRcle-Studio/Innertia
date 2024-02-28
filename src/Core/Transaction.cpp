// 
// Created by qulop on 24.02.2024
// 
#include <Core/Transaction.h>


namespace IR_CORE_NS {
// TXInput
	TXInput::TXInput(IR_TYPES_NS::HashPtr out, uint32_t sequence)
		: m_out(out)
		, m_sequence(sequence)
	{}

	IR_TYPES_NS::HashPtr TXInput::GetOut() const {
		return m_out;
	}

	uint32_t TXInput::GetSequence() const {
		return m_sequence;
	}


	bool TXInput::IsTransactionCompete() const {
		return m_sequence == 0xFFFFFFFF;
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
	IR_TYPES_NS::HashPtr Transaction::GetHash() const {
		return m_hash;
	}


	IR_TYPES_NS::HashPtr Transaction::CalculateHash() {
		return nullptr;
	}


	bool Transaction::IsCoinbase() const {
		return (inputs.size() == 1) && (inputs.at(0).GetOut() == nullptr);
	}

}
