/*
 * Copyright (c) 2023 Google LLC
 *
 * SPDX-License-Identifier: Apache-2.0
 */

// https://developerhelp.microchip.com/xwiki/bin/view/products/mcu-mpu/32bit-mcu/sam/samd21-mcu-overview/peripherals/pm/

#include <soc.h>
#include <samd21g18a.h>
#include <zephyr/drivers/counter.h>
#include <zephyr/drivers/interrupt_controller/gic.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/pm/pm.h>
#include <zephyr/init.h>

LOG_MODULE_DECLARE(soc, CONFIG_SOC_LOG_LEVEL);


void pm_state_set(enum pm_state state, uint8_t substate_id)
{
	ARG_UNUSED(substate_id);

	switch (state) {
	case PM_STATE_STANDBY:
	{
		/* Configure Standby Sleep */
		SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
		/* Wait for interrupt instruction execution */
		__WFI();
	}
		break;
	case PM_STATE_RUNTIME_IDLE:
	{
		/* Configure Idle Sleep mode */
		SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;
		PM->SLEEP.bit.IDLE = PM_SLEEP_IDLE_CPU_Val;
		/* Wait for interrupt instruction execution */	
		__WFI();
	}
		break;
	default:
		LOG_DBG("Unsupported power state %u", state);
		break;
	}
}

void pm_state_exit_post_ops(enum pm_state state, uint8_t substate_id)
{
	ARG_UNUSED(substate_id);
	uint8_t reset_cause = PM->RCAUSE.reg;
	LOG_DBG("Reset cause: %u", reset_cause);
	switch (state) {
	case PM_STATE_STANDBY:
	{

	}
		break;
	case PM_STATE_RUNTIME_IDLE:
	{

	}
		break;
	default:
		LOG_DBG("Unsupported power state %u", state);
		break;
	}

	/*
	 * System is now in active mode. Reenable interrupts which were
	 * disabled when OS started idling code.
	 */
	irq_unlock(0);
}