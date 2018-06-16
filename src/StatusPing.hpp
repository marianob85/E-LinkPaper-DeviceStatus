#include <vector>
#include <map>
#include <thread>
#include <experimental/filesystem>
#include "StatusManager.hpp"

class StatusPing : public StatusPage
{
public:
	StatusPing( std::experimental::filesystem::path xmlPath );

	// start: ------------------- StatusPage -------------------
	virtual std::unique_ptr< Paint > currentPage( size_t width, size_t height ) const override;
	virtual bool setPage( unsigned page ) override;
	virtual bool setNext() override;
	virtual std::string getDescription( size_t index ) const override;
	virtual unsigned pageNo() const override;
	virtual unsigned currentPageNo() const override;
	// end: --------------------- StatusPage -------------------

private:
	void pinger();
	std::map< std::string, bool > getDeviceStatusV2( bool clear = false ) const;

private:
	unsigned m_pingCount{ 4 };
	struct Device
	{
		std::string Ip;
		std::string Name;
	};
	mutable std::vector< Device > m_devices;
	mutable std::mutex m_statusMutex;
	mutable std::map< std::string, bool > m_status;
	std::thread m_pinger;
};