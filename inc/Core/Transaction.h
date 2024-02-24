// 
// Created by qulop on 24.02.2024
// 

#ifndef INNERTIA_TRANSACTION_H
#define INNERTIA_TRANSACTION_H

#include <macros.h>

#include <Utils/Types/MerkleTree.h>
#include <Utils/stdInclude.h>


namespace IR_CORE_NS {
	using IRHash = SR_TYPES_NS::HashT<256>;
	using HashPtr = std::shared_ptr<IRHash>;


	// Transaction input
	class TXInput {
	public:
		TXInput(HashPtr out, uint32_t sequence);

		IR_NODISCARD HashPtr GetOut() const;

		IR_NODISCARD uint32_t GetSequence() const;

	private:
		HashPtr m_out = nullptr;
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

		IR_NODISCARD HashPtr GetHash() const;

		HashPtr ComputeHash();

	public:
		// For now transaction can contain only one input and output.
		std::vector<TXInput> inputs;	// previous transactions
		std::vector<TXOutput> outputs;	// ...and of subsiquent

	private:
		float m_value = 0;
		HashPtr m_hash = nullptr;		// Hash of current transaction
		
		const uint32_t m_lockTime = 0;
	};

}


#endif // INNERTIA_TRANSACTION_H