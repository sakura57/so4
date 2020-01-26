#pragma once

#include <mutex>
#include <list>

#include "InterfacePanel.hxx"
#include "ITransientStateStructure.hxx"
#include "CAudioManager.hxx"

class InterfaceManager : public ITransientStateStructure
{
public:
	InterfaceManager();
	~InterfaceManager();

	/*
	 * Render all panels owned by the manager.
	 * 
	 * This should occur during the render loop
	 * before the window has been cleared but after
	 * world objects have been rendered.
	 */
	void render_all_panels(float const);

	/*
	 * Release all panels (invoking delete on them)
	 * owned by the manager.
	 * 
	 * This function should *always* be invoked when the
	 * current GUI is no longer needed, including game exit
	 */
	void free_all_panels(void);

	/*
	 * We take ownership of an interface panel, and it becomes
	 * visible on the next render tick.
	 */
	void add_panel(InterfacePanel *);

	//inherited from ITransientStateStructure
	virtual void shifting_out(void) final;

	/*
	 * Get the number of panels managed by the manager
	 */
	virtual size_t get_num_panels(void) final;

	void register_tangible_ui_event(void);

	bool input_blocked(void);

private:
	std::mutex m_mFieldAccess;
	std::mutex m_mQueueAccess;
	std::list<InterfacePanel *> m_vPanels;
	std::list<InterfacePanel *> m_vPendingPanels;
	ImFont *m_pFont;
	CAudioManager *m_pAudioManager;
	int m_iInputHinderingPanelCount;
};