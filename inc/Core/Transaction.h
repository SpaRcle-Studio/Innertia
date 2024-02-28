// 
// Created by qulop on 24.02.2024
// 

#ifndef INNERTIA_TRANSACTION_H
#define INNERTIA_TRANSACTION_H

#include <macros.h>
#include <Utils/Types/Hash.h>

#include <Utils/stdInclude.h>


namespace IR_CORE_NS {
	// Transaction input
	class TXInput {
	public:
		TXInput(IR_TYPES_NS::HashPtr out, uint32_t sequence);

		IR_NODISCARD IR_TYPES_NS::HashPtr GetOut() const;

		IR_NODISCARD uint32_t GetSequence() const;

		bool IsTransactionCompete() const;

	private:
		IR_TYPES_NS::HashPtr m_out = nullptr;
		uint32_t m_sequence = 0;
	};


	// Transaction output
	class TXOutput {
	public:
		TXOutput(float value);

		IR_NODISCARD float GetValue() const;

	private:
		float m_value;
	};



	class Transaction {
	public:
		Transaction() = default;

		IR_NODISCARD IR_TYPES_NS::HashPtr GetHash() const;

		IR_TYPES_NS::HashPtr CalculateHash();

		bool IsCoinbase() const;

	public:
		std::vector<TXInput> inputs;	// previous transactions
		std::vector<TXOutput> outputs;	// ...and of subsiquent

	private:
		IR_TYPES_NS::HashPtr m_hash = nullptr;		// Hash of current transaction
			
		const uint32_t m_lockTime = 0;	// The time until which the transaction will be frozen (～￣▽￣)～
	};

}


#endif // INNERTIA_TRANSACTION_H