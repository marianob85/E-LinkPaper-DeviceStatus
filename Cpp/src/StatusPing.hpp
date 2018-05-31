#include <vector>
#include <map>
#include <experimental/filesystem>
#include "StatusManager.hpp"
#include "DeviceDef.hpp"

class StatusPing : public StatusPage
{
public:
	StatusPing( std::experimental::filesystem::path xmlPath );
	// start: ------------------- StatusPage -------------------
	virtual Paint currentPage() const override;
	virtual bool setPage( unsigned page ) override;
	virtual bool setNext() override;
	virtual std::string getDescription() const override;
	virtual unsigned pageNo() const override;
	virtual unsigned currentPageNo() const override;
	// end: --------------------- StatusPage -------------------

private:
	std::map< std::string, bool > getDeviceStatus() const;

private:
	unsigned m_currentPage{ 0 };
	unsigned m_pingCount{ 4 };
	struct Device
	{
		std::string Ip;
		std::string Name;
	};
	std::vector< Device > m_devices;
};