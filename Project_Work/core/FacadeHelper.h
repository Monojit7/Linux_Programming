#ifndef _FACADE_HELPER_H_
#define _FACADE_HELPER_H_

template <class T>
class FacadeHelper
{
public : 
	FacadeHelper()
	{
		if (nullptr == m_pEngine)
		{
			m_pEngine = new T; 
		}
		mClientID = m_pEngine->registerClient(); 
	}

	~FacadeHelper()
	{
		m_pEngine->deregisterClient(mClientID);
	}

	T* operator->()
	{
		return m_pEngine; 
	}

	uint32_t  mClientID;

private : 
	static T* m_pEngine; 
};

template <class T>
T* FacadeHelper<T>::m_pEngine = nullptr; 

#endif