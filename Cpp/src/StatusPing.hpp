#include "StatusManager.hpp"

class StatusPing : public StatusPage
{
public:
	// start: ------------------- StatusPage -------------------
	virtual Paint currentPage() const override;
	virtual bool setPage( unsigned page ) override;
	virtual bool setNext() override;
	virtual std::string getDescription() const override;
	virtual unsigned pageNo() const override;
	virtual unsigned currentPageNo() const override;
	// end: --------------------- StatusPage -------------------
};