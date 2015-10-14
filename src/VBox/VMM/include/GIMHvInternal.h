/* $Id$ */
/** @file
 * GIM - Hyper-V, Internal header file.
 */

/*
 * Copyright (C) 2014-2015 Oracle Corporation
 *
 * This file is part of VirtualBox Open Source Edition (OSE), as
 * available from http://www.virtualbox.org. This file is free software;
 * you can redistribute it and/or modify it under the terms of the GNU
 * General Public License (GPL) as published by the Free Software
 * Foundation, in version 2 as it comes in the "COPYING" file of the
 * VirtualBox OSE distribution. VirtualBox OSE is distributed in the
 * hope that it will be useful, but WITHOUT ANY WARRANTY of any kind.
 */

#ifndef ___GIMHvInternal_h
#define ___GIMHvInternal_h

#include <VBox/vmm/gim.h>
#include <VBox/vmm/cpum.h>

#include <iprt/net.h>

/** @name Hyper-V base feature identification.
 * Features based on current partition privileges (per-VM).
 * @{
 */
/** Virtual processor runtime MSR available. */
#define GIM_HV_BASE_FEAT_VP_RUNTIME_MSR           RT_BIT(0)
/** Partition reference counter MSR available. */
#define GIM_HV_BASE_FEAT_PART_TIME_REF_COUNT_MSR  RT_BIT(1)
/** Basic Synthetic Interrupt Controller MSRs available. */
#define GIM_HV_BASE_FEAT_BASIC_SYNIC_MSRS         RT_BIT(2)
/** Synthetic Timer MSRs available. */
#define GIM_HV_BASE_FEAT_STIMER_MSRS              RT_BIT(3)
/** APIC access MSRs (EOI, ICR, TPR) available. */
#define GIM_HV_BASE_FEAT_APIC_ACCESS_MSRS         RT_BIT(4)
/** Hypercall MSRs available. */
#define GIM_HV_BASE_FEAT_HYPERCALL_MSRS           RT_BIT(5)
/** Access to VCPU index MSR available. */
#define GIM_HV_BASE_FEAT_VP_ID_MSR                RT_BIT(6)
/** Virtual system reset MSR available. */
#define GIM_HV_BASE_FEAT_VIRT_SYS_RESET_MSR       RT_BIT(7)
/** Statistic pages MSRs available. */
#define GIM_HV_BASE_FEAT_STAT_PAGES_MSR           RT_BIT(8)
/** Paritition reference TSC MSR available. */
#define GIM_HV_BASE_FEAT_PART_REF_TSC_MSR         RT_BIT(9)
/** Virtual guest idle state MSR available. */
#define GIM_HV_BASE_FEAT_GUEST_IDLE_STATE_MSR     RT_BIT(10)
/** Timer frequency MSRs (TSC and APIC) available. */
#define GIM_HV_BASE_FEAT_TIMER_FREQ_MSRS          RT_BIT(11)
/** Debug MSRs available. */
#define GIM_HV_BASE_FEAT_DEBUG_MSRS               RT_BIT(12)
/** @}  */

/** @name Hyper-V partition-creation feature identification.
 * Indicates flags specified during partition creation.
 * @{
 */
/** Create partitions. */
#define GIM_HV_PART_FLAGS_CREATE_PART             RT_BIT(0)
/** Access partition Id. */
#define GIM_HV_PART_FLAGS_ACCESS_PART_ID          RT_BIT(1)
/** Access memory pool. */
#define GIM_HV_PART_FLAGS_ACCESS_MEMORY_POOL      RT_BIT(2)
/** Adjust message buffers. */
#define GIM_HV_PART_FLAGS_ADJUST_MSG_BUFFERS      RT_BIT(3)
/** Post messages. */
#define GIM_HV_PART_FLAGS_POST_MSGS               RT_BIT(4)
/** Signal events. */
#define GIM_HV_PART_FLAGS_SIGNAL_EVENTS           RT_BIT(5)
/** Create port. */
#define GIM_HV_PART_FLAGS_CREATE_PORT             RT_BIT(6)
/** Connect port. */
#define GIM_HV_PART_FLAGS_CONNECT_PORT            RT_BIT(7)
/** Access statistics. */
#define GIM_HV_PART_FLAGS_ACCESS_STATS            RT_BIT(8)
/** Debugging.*/
#define GIM_HV_PART_FLAGS_DEBUGGING               RT_BIT(11)
/** CPU management. */
#define GIM_HV_PART_FLAGS_CPU_MGMT                RT_BIT(12)
/** CPU profiler. */
#define GIM_HV_PART_FLAGS_CPU_PROFILER            RT_BIT(13)
/** Enable expanded stack walking. */
#define GIM_HV_PART_FLAGS_EXPANDED_STACK_WALK     RT_BIT(14)
/** @}  */

/** @name Hyper-V power management feature identification.
 * @{
 */
/** Maximum CPU power state C0. */
#define GIM_HV_PM_MAX_CPU_POWER_STATE_C0          RT_BIT(0)
/** Maximum CPU power state C1. */
#define GIM_HV_PM_MAX_CPU_POWER_STATE_C1          RT_BIT(1)
/** Maximum CPU power state C2. */
#define GIM_HV_PM_MAX_CPU_POWER_STATE_C2          RT_BIT(2)
/** Maximum CPU power state C3. */
#define GIM_HV_PM_MAX_CPU_POWER_STATE_C3          RT_BIT(3)
/** HPET is required to enter C3 power state. */
#define GIM_HV_PM_HPET_REQD_FOR_C3                RT_BIT(4)
/** @}  */

/** @name Hyper-V miscellaneous feature identification.
 * Miscellaneous features available for the current partition.
 * @{
 */
/** MWAIT instruction available. */
#define GIM_HV_MISC_FEAT_MWAIT                    RT_BIT(0)
/** Guest debugging support available. */
#define GIM_HV_MISC_FEAT_GUEST_DEBUGGING          RT_BIT(1)
/** Performance monitor support is available. */
#define GIM_HV_MISC_FEAT_PERF_MON                 RT_BIT(2)
/** Support for physical CPU dynamic partitioning events. */
#define GIM_HV_MISC_FEAT_PCPU_DYN_PART_EVENT      RT_BIT(3)
/** Support for passing hypercall input parameter block via XMM registers. */
#define GIM_HV_MISC_FEAT_XMM_HYPERCALL_INPUT      RT_BIT(4)
/** Support for virtual guest idle state. */
#define GIM_HV_MISC_FEAT_GUEST_IDLE_STATE         RT_BIT(5)
/** Support for hypervisor sleep state. */
#define GIM_HV_MISC_FEAT_HYPERVISOR_SLEEP_STATE   RT_BIT(6)
/** Support for querying NUMA distances. */
#define GIM_HV_MISC_FEAT_QUERY_NUMA_DISTANCE      RT_BIT(7)
/** Support for determining timer frequencies. */
#define GIM_HV_MISC_FEAT_TIMER_FREQ               RT_BIT(8)
/** Support for injecting synthetic machine checks. */
#define GIM_HV_MISC_FEAT_INJECT_SYNMC_XCPT        RT_BIT(9)
/** Support for guest crash MSRs. */
#define GIM_HV_MISC_FEAT_GUEST_CRASH_MSRS         RT_BIT(10)
/** Support for debug MSRs. */
#define GIM_HV_MISC_FEAT_DEBUG_MSRS               RT_BIT(11)
/** Npiep1 Available */ /** @todo What the heck is this? */
#define GIM_HV_MISC_FEAT_NPIEP1                   RT_BIT(12)
/** Disable hypervisor available. */
#define GIM_HV_MISC_FEAT_DISABLE_HYPERVISOR       RT_BIT(13)
/** @}  */

/** @name Hyper-V implementation recommendations.
 * Recommendations from the hypervisor for the guest for optimal performance.
 * @{
 */
/** Use hypercall for address space switches rather than MOV CR3. */
#define GIM_HV_HINT_HYPERCALL_FOR_PROCESS_SWITCH  RT_BIT(0)
/** Use hypercall for local TLB flushes rather than INVLPG/MOV CR3. */
#define GIM_HV_HINT_HYPERCALL_FOR_TLB_FLUSH       RT_BIT(1)
/** Use hypercall for inter-CPU TLB flushes rather than IPIs. */
#define GIM_HV_HINT_HYPERCALL_FOR_TLB_SHOOTDOWN   RT_BIT(2)
/** Use MSRs for APIC access (EOI, ICR, TPR) rather than MMIO. */
#define GIM_HV_HINT_MSR_FOR_APIC_ACCESS           RT_BIT(3)
/** Use hypervisor provided MSR for a system reset. */
#define GIM_HV_HINT_MSR_FOR_SYS_RESET             RT_BIT(4)
/** Relax timer-related checks (watchdogs/deadman timeouts) that rely on
 *  timely deliver of external interrupts. */
#define GIM_HV_HINT_RELAX_TIME_CHECKS             RT_BIT(5)
/** Use DMA remapping. */
#define GIM_HV_HINT_DMA_REMAPPING                 RT_BIT(6)
/** Use interrupt remapping. */
#define GIM_HV_HINT_INTERRUPT_REMAPPING           RT_BIT(7)
/** Use X2APIC MSRs rather than MMIO. */
#define GIM_HV_HINT_X2APIC_MSRS                   RT_BIT(8)
/** Deprecate Auto EOI (end of interrupt). */
#define GIM_HV_HINT_DEPRECATE_AUTO_EOI            RT_BIT(9)
/** @}  */


/** @name Hyper-V implementation hardware features.
 * Which hardware features are in use by the hypervisor.
 * @{
 */
/** APIC overlay is used. */
#define GIM_HV_HOST_FEAT_AVIC                     RT_BIT(0)
/** MSR bitmaps is used. */
#define GIM_HV_HOST_FEAT_MSR_BITMAP               RT_BIT(1)
/** Architectural performance counter supported. */
#define GIM_HV_HOST_FEAT_PERF_COUNTER             RT_BIT(2)
/** Nested paging is used. */
#define GIM_HV_HOST_FEAT_NESTED_PAGING            RT_BIT(3)
/** DMA remapping is used. */
#define GIM_HV_HOST_FEAT_DMA_REMAPPING            RT_BIT(4)
/** Interrupt remapping is used. */
#define GIM_HV_HOST_FEAT_INTERRUPT_REMAPPING      RT_BIT(5)
/** Memory patrol scrubber is present. */
#define GIM_HV_HOST_FEAT_MEM_PATROL_SCRUBBER      RT_BIT(6)
/** @}  */


/** @name Hyper-V MSRs.
 * @{
 */
/** Start of range 0. */
#define MSR_GIM_HV_RANGE0_START                   UINT32_C(0x40000000)
/** Guest OS identification (R/W) */
#define MSR_GIM_HV_GUEST_OS_ID                    UINT32_C(0x40000000)
/** Enable hypercall interface (R/W) */
#define MSR_GIM_HV_HYPERCALL                      UINT32_C(0x40000001)
/** Virtual processor's (VCPU) index (R) */
#define MSR_GIM_HV_VP_INDEX                       UINT32_C(0x40000002)
/** Reset operation (R/W) */
#define MSR_GIM_HV_RESET                          UINT32_C(0x40000003)
/** End of range 0. */
#define MSR_GIM_HV_RANGE0_END                     MSR_GIM_HV_RESET

/** Start of range 1. */
#define MSR_GIM_HV_RANGE1_START                   UINT32_C(0x40000010)
/** Virtual processor's (VCPU) runtime (R) */
#define MSR_GIM_HV_VP_RUNTIME                     UINT32_C(0x40000010)
/** End of range 1. */
#define MSR_GIM_HV_RANGE1_END                     MSR_GIM_HV_VP_RUNTIME

/** Start of range 2. */
#define MSR_GIM_HV_RANGE2_START                   UINT32_C(0x40000020)
/** Per-VM reference counter (R) */
#define MSR_GIM_HV_TIME_REF_COUNT                 UINT32_C(0x40000020)
/** Per-VM TSC page (R/W) */
#define MSR_GIM_HV_REF_TSC                        UINT32_C(0x40000021)
/** Frequency of TSC in Hz as reported by the hypervisor (R) */
#define MSR_GIM_HV_TSC_FREQ                       UINT32_C(0x40000022)
/** Frequency of LAPIC in Hz as reported by the hypervisor (R) */
#define MSR_GIM_HV_APIC_FREQ                      UINT32_C(0x40000023)
/** End of range 2. */
#define MSR_GIM_HV_RANGE2_END                     MSR_GIM_HV_APIC_FREQ

/** Start of range 3. */
#define MSR_GIM_HV_RANGE3_START                   UINT32_C(0x40000070)
/** Access to APIC EOI (End-Of-Interrupt) register (W) */
#define MSR_GIM_HV_EOI                            UINT32_C(0x40000070)
/** Access to APIC ICR (Interrupt Command) register (R/W) */
#define MSR_GIM_HV_ICR                            UINT32_C(0x40000071)
/** Access to APIC TPR (Task Priority) register (R/W) */
#define MSR_GIM_HV_TPR                            UINT32_C(0x40000072)
/** Enables lazy EOI processing (R/W) */
#define MSR_GIM_HV_APIC_ASSIST_PAGE               UINT32_C(0x40000073)
/** End of range 3. */
#define MSR_GIM_HV_RANGE3_END                     MSR_GIM_HV_APIC_ASSIST_PAGE

/** Start of range 4. */
#define MSR_GIM_HV_RANGE4_START                   UINT32_C(0x40000080)
/** Control behaviour of synthetic interrupt controller (R/W) */
#define MSR_GIM_HV_SCONTROL                       UINT32_C(0x40000080)
/** Synthetic interrupt controller version (R) */
#define MSR_GIM_HV_SVERSION                       UINT32_C(0x40000081)
/** Base address of synthetic interrupt event flag (R/W) */
#define MSR_GIM_HV_SIEFP                          UINT32_C(0x40000082)
/** Base address of synthetic interrupt message page (R/W) */
#define MSR_GIM_HV_SIMP                           UINT32_C(0x40000083)
/** End-Of-Message in synthetic interrupt parameter page (W) */
#define MSR_GIM_HV_EOM                            UINT32_C(0x40000084)
/** End of range 4. */
#define MSR_GIM_HV_RANGE4_END                     MSR_GIM_HV_EOM

/** Start of range 5. */
#define MSR_GIM_HV_RANGE5_START                   UINT32_C(0x40000090)
/** Configures synthetic interrupt source 0 (R/W) */
#define MSR_GIM_HV_SINT0                          UINT32_C(0x40000090)
/** Configures synthetic interrupt source 1 (R/W) */
#define MSR_GIM_HV_SINT1                          UINT32_C(0x40000091)
/** Configures synthetic interrupt source 2 (R/W) */
#define MSR_GIM_HV_SINT2                          UINT32_C(0x40000092)
/** Configures synthetic interrupt source 3 (R/W) */
#define MSR_GIM_HV_SINT3                          UINT32_C(0x40000093)
/** Configures synthetic interrupt source 4 (R/W) */
#define MSR_GIM_HV_SINT4                          UINT32_C(0x40000094)
/** Configures synthetic interrupt source 5 (R/W) */
#define MSR_GIM_HV_SINT5                          UINT32_C(0x40000095)
/** Configures synthetic interrupt source 6 (R/W) */
#define MSR_GIM_HV_SINT6                          UINT32_C(0x40000096)
/** Configures synthetic interrupt source 7 (R/W) */
#define MSR_GIM_HV_SINT7                          UINT32_C(0x40000097)
/** Configures synthetic interrupt source 8 (R/W) */
#define MSR_GIM_HV_SINT8                          UINT32_C(0x40000098)
/** Configures synthetic interrupt source 9 (R/W) */
#define MSR_GIM_HV_SINT9                          UINT32_C(0x40000099)
/** Configures synthetic interrupt source 10 (R/W) */
#define MSR_GIM_HV_SINT10                         UINT32_C(0x4000009A)
/** Configures synthetic interrupt source 11 (R/W) */
#define MSR_GIM_HV_SINT11                         UINT32_C(0x4000009B)
/** Configures synthetic interrupt source 12 (R/W) */
#define MSR_GIM_HV_SINT12                         UINT32_C(0x4000009C)
/** Configures synthetic interrupt source 13 (R/W) */
#define MSR_GIM_HV_SINT13                         UINT32_C(0x4000009D)
/** Configures synthetic interrupt source 14 (R/W) */
#define MSR_GIM_HV_SINT14                         UINT32_C(0x4000009E)
/** Configures synthetic interrupt source 15 (R/W) */
#define MSR_GIM_HV_SINT15                         UINT32_C(0x4000009F)
/** End of range 5. */
#define MSR_GIM_HV_RANGE5_END                     MSR_GIM_HV_SINT15

/** Start of range 6. */
#define MSR_GIM_HV_RANGE6_START                   UINT32_C(0x400000B0)
/** Configures register for synthetic timer 0 (R/W) */
#define MSR_GIM_HV_STIMER0_CONFIG                 UINT32_C(0x400000B0)
/** Expiration time or period for synthetic timer 0 (R/W) */
#define MSR_GIM_HV_STIMER0_COUNT                  UINT32_C(0x400000B1)
/** Configures register for synthetic timer 1 (R/W) */
#define MSR_GIM_HV_STIMER1_CONFIG                 UINT32_C(0x400000B2)
/** Expiration time or period for synthetic timer 1 (R/W) */
#define MSR_GIM_HV_STIMER1_COUNT                  UINT32_C(0x400000B3)
/** Configures register for synthetic timer 2 (R/W) */
#define MSR_GIM_HV_STIMER2_CONFIG                 UINT32_C(0x400000B4)
/** Expiration time or period for synthetic timer 2 (R/W) */
#define MSR_GIM_HV_STIMER2_COUNT                  UINT32_C(0x400000B5)
/** Configures register for synthetic timer 3 (R/W) */
#define MSR_GIM_HV_STIMER3_CONFIG                 UINT32_C(0x400000B6)
/** Expiration time or period for synthetic timer 3 (R/W) */
#define MSR_GIM_HV_STIMER3_COUNT                  UINT32_C(0x400000B7)
/** End of range 6. */
#define MSR_GIM_HV_RANGE6_END                     MSR_GIM_HV_STIMER3_COUNT

/** Start of range 7. */
#define MSR_GIM_HV_RANGE7_START                   UINT32_C(0x400000C1)
/** Trigger to transition to power state C1 (R) */
#define MSR_GIM_HV_POWER_STATE_TRIGGER_C1         UINT32_C(0x400000C1)
/** Trigger to transition to power state C2 (R) */
#define MSR_GIM_HV_POWER_STATE_TRIGGER_C2         UINT32_C(0x400000C2)
/** Trigger to transition to power state C3 (R) */
#define MSR_GIM_HV_POWER_STATE_TRIGGER_C3         UINT32_C(0x400000C3)
/** End of range 7. */
#define MSR_GIM_HV_RANGE7_END                     MSR_GIM_HV_POWER_STATE_TRIGGER_C3

/** Start of range 8. */
#define MSR_GIM_HV_RANGE8_START                   UINT32_C(0x400000D1)
/** Configure the recipe for power state transitions to C1 (R/W) */
#define MSR_GIM_HV_POWER_STATE_CONFIG_C1          UINT32_C(0x400000D1)
/** Configure the recipe for power state transitions to C2 (R/W) */
#define MSR_GIM_HV_POWER_STATE_CONFIG_C2          UINT32_C(0x400000D2)
/** Configure the recipe for power state transitions to C3 (R/W) */
#define MSR_GIM_HV_POWER_STATE_CONFIG_C3          UINT32_C(0x400000D3)
/** End of range 8. */
#define MSR_GIM_HV_RANGE8_END                     MSR_GIM_HV_POWER_STATE_CONFIG_C3

/** Start of range 9. */
#define MSR_GIM_HV_RANGE9_START                   UINT32_C(0x400000E0)
/** Map the guest's retail partition stats page (R/W) */
#define MSR_GIM_HV_STATS_PART_RETAIL_PAGE         UINT32_C(0x400000E0)
/** Map the guest's internal partition stats page (R/W) */
#define MSR_GIM_HV_STATS_PART_INTERNAL_PAGE       UINT32_C(0x400000E1)
/** Map the guest's retail VP stats page (R/W) */
#define MSR_GIM_HV_STATS_VP_RETAIL_PAGE           UINT32_C(0x400000E2)
/** Map the guest's internal VP stats page (R/W) */
#define MSR_GIM_HV_STATS_VP_INTERNAL_PAGE         UINT32_C(0x400000E3)
/** End of range 9. */
#define MSR_GIM_HV_RANGE9_END                     MSR_GIM_HV_STATS_VP_INTERNAL_PAGE

/** Start of range 10. */
#define MSR_GIM_HV_RANGE10_START                  UINT32_C(0x400000F0)
/** Trigger the guest's transition to idle power state (R) */
#define MSR_GIM_HV_GUEST_IDLE                     UINT32_C(0x400000F0)
/** Synthetic debug control. */
#define MSR_GIM_HV_SYNTH_DEBUG_CONTROL            UINT32_C(0x400000F1)
/** Synthetic debug status. */
#define MSR_GIM_HV_SYNTH_DEBUG_STATUS             UINT32_C(0x400000F2)
/** Synthetic debug send buffer. */
#define MSR_GIM_HV_SYNTH_DEBUG_SEND_BUFFER        UINT32_C(0x400000F3)
/** Synthetic debug receive buffer. */
#define MSR_GIM_HV_SYNTH_DEBUG_RECEIVE_BUFFER     UINT32_C(0x400000F4)
/** Synthetic debug pending buffer. */
#define MSR_GIM_HV_SYNTH_DEBUG_PENDING_BUFFER     UINT32_C(0x400000F5)
/** End of range 10. */
#define MSR_GIM_HV_RANGE10_END                    MSR_GIM_HV_SYNTH_DEBUG_PENDING_BUFFER

/** Start of range 11. */
#define MSR_GIM_HV_RANGE11_START                  UINT32_C(0x400000FF)
/** Undocumented debug options MSR. */
#define MSR_GIM_HV_DEBUG_OPTIONS_MSR              UINT32_C(0x400000FF)
/** End of range 11. */
#define MSR_GIM_HV_RANGE11_END                    MSR_GIM_HV_DEBUG_OPTIONS_MSR

/** Start of range 12. */
#define MSR_GIM_HV_RANGE12_START                  UINT32_C(0x40000100)
/** Guest crash MSR 0. */
#define MSR_GIM_HV_CRASH_P0                       UINT32_C(0x40000100)
/** Guest crash MSR 1. */
#define MSR_GIM_HV_CRASH_P1                       UINT32_C(0x40000101)
/** Guest crash MSR 2. */
#define MSR_GIM_HV_CRASH_P2                       UINT32_C(0x40000102)
/** Guest crash MSR 3. */
#define MSR_GIM_HV_CRASH_P3                       UINT32_C(0x40000103)
/** Guest crash MSR 4. */
#define MSR_GIM_HV_CRASH_P4                       UINT32_C(0x40000104)
/** Guest crash control. */
#define MSR_GIM_HV_CRASH_CTL                      UINT32_C(0x40000105)
/** End of range 12. */
#define MSR_GIM_HV_RANGE12_END                    MSR_GIM_HV_CRASH_CTL
/** @} */

AssertCompile(MSR_GIM_HV_RANGE0_START  <= MSR_GIM_HV_RANGE0_END);
AssertCompile(MSR_GIM_HV_RANGE1_START  <= MSR_GIM_HV_RANGE1_END);
AssertCompile(MSR_GIM_HV_RANGE2_START  <= MSR_GIM_HV_RANGE2_END);
AssertCompile(MSR_GIM_HV_RANGE3_START  <= MSR_GIM_HV_RANGE3_END);
AssertCompile(MSR_GIM_HV_RANGE4_START  <= MSR_GIM_HV_RANGE4_END);
AssertCompile(MSR_GIM_HV_RANGE5_START  <= MSR_GIM_HV_RANGE5_END);
AssertCompile(MSR_GIM_HV_RANGE6_START  <= MSR_GIM_HV_RANGE6_END);
AssertCompile(MSR_GIM_HV_RANGE7_START  <= MSR_GIM_HV_RANGE7_END);
AssertCompile(MSR_GIM_HV_RANGE8_START  <= MSR_GIM_HV_RANGE8_END);
AssertCompile(MSR_GIM_HV_RANGE9_START  <= MSR_GIM_HV_RANGE9_END);
AssertCompile(MSR_GIM_HV_RANGE10_START <= MSR_GIM_HV_RANGE10_END);
AssertCompile(MSR_GIM_HV_RANGE11_START <= MSR_GIM_HV_RANGE11_END);

/** @name Hyper-V MSR - Reset (MSR_GIM_HV_RESET).
 * @{
 */
/** The hypercall enable bit. */
#define MSR_GIM_HV_RESET_BIT                      RT_BIT_64(0)
/** Whether the hypercall-page is enabled or not. */
#define MSR_GIM_HV_RESET_IS_SET(a)                RT_BOOL((a) & MSR_GIM_HV_RESET_BIT)
/** @} */

/** @name Hyper-V MSR - Hypercall (MSR_GIM_HV_HYPERCALL).
 * @{
 */
/** Guest-physical page frame number of the hypercall-page. */
#define MSR_GIM_HV_HYPERCALL_GUEST_PFN(a)         ((a) >> 12)
/** The hypercall enable bit. */
#define MSR_GIM_HV_HYPERCALL_PAGE_ENABLE_BIT      RT_BIT_64(0)
/** Whether the hypercall-page is enabled or not. */
#define MSR_GIM_HV_HYPERCALL_PAGE_IS_ENABLED(a)   RT_BOOL((a) & MSR_GIM_HV_HYPERCALL_PAGE_ENABLE_BIT)
/** @} */

/** @name Hyper-V MSR - Reference TSC (MSR_GIM_HV_REF_TSC).
 * @{
 */
/** Guest-physical page frame number of the TSC-page. */
#define MSR_GIM_HV_REF_TSC_GUEST_PFN(a)           ((a) >> 12)
/** The TSC-page enable bit. */
#define MSR_GIM_HV_REF_TSC_ENABLE_BIT             RT_BIT_64(0)
/** Whether the TSC-page is enabled or not. */
#define MSR_GIM_HV_REF_TSC_IS_ENABLED(a)          RT_BOOL((a) & MSR_GIM_HV_REF_TSC_ENABLE_BIT)
/** @} */

/** @name Hyper-V MSR - Guest crash control (MSR_GIM_HV_CRASH_CTL).
 * @{
 */
/** The Crash Notify bit. */
#define MSR_GIM_HV_CRASH_CTL_NOTIFY_BIT           RT_BIT_64(63)
/** @} */

/** @name Hyper-V MSR - Guest OS ID (MSR_GIM_HV_GUEST_OS_ID).
 * @{
 */
/** An open-source operating system. */
#define MSR_GIM_HV_GUEST_OS_ID_IS_OPENSOURCE(a)   RT_BOOL((a) & RT_BIT_64(63))
/** Vendor ID. */
#define MSR_GIM_HV_GUEST_OS_ID_VENDOR(a)          (((a) >> 48) & 0xfff)
/** Guest OS variant, depending on the vendor ID.  */
#define MSR_GIM_HV_GUEST_OS_ID_OS_VARIANT(a)      (((a) >> 40) & 0xff)
/** Guest OS major version. */
#define MSR_GIM_HV_GUEST_OS_ID_MAJOR_VERSION(a)   (((a) >> 32) & 0xff)
/** Guest OS minor version. */
#define MSR_GIM_HV_GUEST_OS_ID_MINOR_VERSION(a)   (((a) >> 24) & 0xff)
/** Guest OS service version (e.g. service pack number in case of Windows). */
#define MSR_GIM_HV_GUEST_OS_ID_SERVICE_VERSION(a) (((a) >> 16) & 0xff)
/** Guest OS build number. */
#define MSR_GIM_HV_GUEST_OS_ID_BUILD(a)           ((a) & 0xffff)
/** @} */

/** @name Hyper-V hypercall op codes.
 * @{
 */
/** Post debug data to hypervisor. */
#define GIM_HV_HYPERCALL_OP_POST_DEBUG_DATA       0x69
/** Retreive debug data from hypervisor. */
#define GIM_HV_HYPERCALL_OP_RETREIVE_DEBUG_DATA   0x6A
/** Reset debug session. */
#define GIM_HV_HYPERCALL_OP_RESET_DEBUG_SESSION   0x6B
/** @} */

/** @name Hyper-V hypercall inputs.
 * @{
 */
/** The hypercall call operation code. */
#define GIM_HV_HYPERCALL_IN_CALL_CODE(a)         ((a) & UINT64_C(0xffff))
/** Whether it's a fast (register based) hypercall or not (memory-based). */
#define GIM_HV_HYPERCALL_IN_IS_FAST(a)           RT_BOOL((a) & RT_BIT_64(16))
/** Total number of reps for a rep hypercall. */
#define GIM_HV_HYPERCALL_IN_REP_COUNT(a)         (((a) << 32) & UINT64_C(0xfff))
/** Rep start index for a rep hypercall. */
#define GIM_HV_HYPERCALL_IN_REP_START_IDX(a)     (((a) << 48) & UINT64_C(0xfff))
/** Reserved bits range 1. */
#define GIM_HV_HYPERCALL_IN_RSVD_1(a)            (((a) << 17) & UINT64_C(0x7fff))
/** Reserved bits range 2. */
#define GIM_HV_HYPERCALL_IN_RSVD_2(a)            (((a) << 44) & UINT64_C(0xf))
/** Reserved bits range 3. */
#define GIM_HV_HYPERCALL_IN_RSVD_3(a)            (((a) << 60) & UINT64_C(0x7))
/** @} */


/** @name Hyper-V hypercall status codes.
 * @{
 */
/** Success. */
#define GIM_HV_STATUS_SUCCESS                                        0x00
/** Unrecognized hypercall. */
#define GIM_HV_STATUS_INVALID_HYPERCALL_CODE                         0x02
/** Invalid hypercall input (rep count, rsvd bits). */
#define GIM_HV_STATUS_INVALID_HYPERCALL_INPUT                        0x03
/** Hypercall guest-physical address not 8-byte aligned or crosses page boundary. */
#define GIM_HV_STATUS_INVALID_ALIGNMENT                              0x04
/** Invalid hypercall parameters. */
#define GIM_HV_STATUS_INVALID_PARAMETER                              0x05
/** Access denied. */
#define GIM_HV_STATUS_ACCESS_DENIED                                  0x06
/** The partition state not valid for specified op. */
#define GIM_HV_STATUS_INVALID_PARTITION_STATE                        0x07
/** The hypercall operation could not be performed. */
#define GIM_HV_STATUS_OPERATION_DENIED                               0x08
/** Specified partition property ID not recognized. */
#define GIM_HV_STATUS_UNKNOWN_PROPERTY                               0x09
/** Specified partition property value not within range. */
#define GIM_HV_STATUS_PROPERTY_VALUE_OUT_OF_RANGE                    0x0a
/** Insufficient memory for performing the hypercall. */
#define GIM_HV_STATUS_INSUFFICIENT_MEMORY                            0x0b
/** Maximum partition depth has been exceeded for the partition hierarchy. */
#define GIM_HV_STATUS_PARTITION_TOO_DEEP                             0x0c
/** The specified partition ID is not valid. */
#define GIM_HV_STATUS_INVALID_PARTITION_ID                           0x0d
/** The specified virtual processor index in invalid. */
#define GIM_HV_STATUS_INVALID_VP_INDEX                               0x0e
/** The specified port ID is not unique or doesn't exist. */
#define GIM_HV_STATUS_INVALID_PORT_ID                                0x11
/** The specified connection ID is not unique or doesn't exist. */
#define GIM_HV_STATUS_INVALID_CONNECTION_ID                          0x12
/** The target port doesn't have sufficient buffers for the caller to post a message. */
#define GIM_HV_STATUS_INSUFFICIENT_BUFFERS                           0x13
/** External interrupt not acknowledged.*/
#define GIM_HV_STATUS_NOT_ACKNOWLEDGED                               0x14
/** External interrupt acknowledged. */
#define GIM_HV_STATUS_ACKNOWLEDGED                                   0x16
/** Invalid state due to misordering Hv[Save|Restore]PartitionState. */
#define GIM_HV_STATUS_INVALID_SAVE_RESTORE_STATE                     0x17
/** Operation not perform due to a required feature of SynIc was disabled. */
#define GIM_HV_STATUS_INVALID_SYNIC_STATE                            0x18
/** Object or value already in use. */
#define GIM_HV_STATUS_OBJECT_IN_USE                                  0x19
/** Invalid proximity domain information. */
#define GIM_HV_STATUS_INVALID_PROXIMITY_DOMAIN_INFO                  0x1A
/** Attempt to retrieve data failed. */
#define GIM_HV_STATUS_NO_DATA                                        0x1B
/** Debug connection has not recieved any new data since the last time. */
#define GIM_HV_STATUS_INACTIVE                                       0x1C
/** A resource is unavailable for allocation. */
#define GIM_HV_STATUS_NO_RESOURCES                                   0x1D
/** A hypervisor feature is not available to the caller. */
#define GIM_HV_STATUS_FEATURE_UNAVAILABLE                            0x1E
/** The debug packet returned is partial due to an I/O error. */
#define GIM_HV_STATUS_PARTIAL_PACKET                                 0x1F
/** Processor feature SSE3 unsupported. */
#define GIM_HV_STATUS_PROC_FEAT_SSE3_NOT_SUPPORTED                   0x20
/** Processor feature LAHSAHF unsupported. */
#define GIM_HV_STATUS_PROC_FEAT_LAHSAHF_NOT_SUPPORTED                0x21
/** Processor feature SSSE3 unsupported. */
#define GIM_HV_STATUS_PROC_FEAT_SSSE3_NOT_SUPPORTED                  0x22
/** Processor feature SSE4.1 unsupported. */
#define GIM_HV_STATUS_PROC_FEAT_SSE4_1_NOT_SUPPORTED                 0x23
/** Processor feature SSE4.2 unsupported. */
#define GIM_HV_STATUS_PROC_FEAT_SSE4_2_NOT_SUPPORTED                 0x24
/** Processor feature SSE4A unsupported. */
#define GIM_HV_STATUS_PROC_FEAT_SSE4A_NOT_SUPPORTED                  0x25
/** Processor feature XOP unsupported. */
#define GIM_HV_STATUS_PROC_FEAT_XOP_NOT_SUPPORTED                    0x26
/** Processor feature POPCNT unsupported. */
#define GIM_HV_STATUS_PROC_FEAT_POPCNT_NOT_SUPPORTED                 0x27
/** Processor feature CMPXCHG16B unsupported. */
#define GIM_HV_STATUS_PROC_FEAT_CMPXCHG16B_NOT_SUPPORTED             0x28
/** Processor feature ALTMOVCR8 unsupported. */
#define GIM_HV_STATUS_PROC_FEAT_ALTMOVCR8_NOT_SUPPORTED              0x29
/** Processor feature LZCNT unsupported. */
#define GIM_HV_STATUS_PROC_FEAT_LZCNT_NOT_SUPPORTED                  0x2A
/** Processor feature misaligned SSE unsupported. */
#define GIM_HV_STATUS_PROC_FEAT_MISALIGNED_SSE_NOT_SUPPORTED         0x2B
/** Processor feature MMX extensions unsupported. */
#define GIM_HV_STATUS_PROC_FEAT_MMX_EXT_NOT_SUPPORTED                0x2C
/** Processor feature 3DNow! unsupported. */
#define GIM_HV_STATUS_PROC_FEAT_3DNOW_NOT_SUPPORTED                  0x2D
/** Processor feature Extended 3DNow! unsupported. */
#define GIM_HV_STATUS_PROC_FEAT_EXTENDED_3DNOW_NOT_SUPPORTED         0x2E
/** Processor feature 1GB large page unsupported. */
#define GIM_HV_STATUS_PROC_FEAT_PAGE_1GB_NOT_SUPPORTED               0x2F
/** Processor cache line flush size incompatible. */
#define GIM_HV_STATUS_PROC_CACHE_LINE_FLUSH_SIZE_INCOMPATIBLE        0x30
/** Processor feature XSAVE unsupported. */
#define GIM_HV_STATUS_PROC_FEAT_XSAVE_NOT_SUPPORTED                  0x31
/** Processor feature XSAVEOPT unsupported. */
#define GIM_HV_STATUS_PROC_FEAT_XSAVEOPT_NOT_SUPPORTED               0x32
/** The specified buffer was too small for all requested data. */
#define GIM_HV_STATUS_INSUFFICIENT_BUFFER                            0x33
/** Processor feature XSAVEOPT unsupported. */
#define GIM_HV_STATUS_PROC_FEAT_XSAVE_AVX_NOT_SUPPORTED              0x34
/** Processor feature XSAVEOPT unsupported. */
#define GIM_HV_STATUS_PROC_FEAT_XSAVE_FEAT_NOT_SUPPORTED             0x35   /** Huh, isn't this same as 0x31? */
/** Processor feature XSAVEOPT unsupported. */
#define GIM_HV_STATUS_PROC_FEAT_PAGE_XSAVE_SAVE_AREA_INCOMPATIBLE    0x36
/** Processor architecture unsupoorted. */
#define GIM_HV_STATUS_INCOMPATIBLE_PROCESSOR                         0x37
/** Max. domains for platform I/O remapping reached. */
#define GIM_HV_STATUS_INSUFFICIENT_DEVICE_DOMAINS                    0x38
/** Processor feature AES unsupported. */
#define GIM_HV_STATUS_PROC_FEAT_AES_NOT_SUPPORTED                    0x39
/** Processor feature PCMULQDQ unsupported. */
#define GIM_HV_STATUS_PROC_FEAT_PCMULQDQ_NOT_SUPPORTED               0x3A
/** Processor feature XSAVE features unsupported. */
#define GIM_HV_STATUS_PROC_FEAT_XSAVE_FEATURES_INCOMPATIBLE          0x3B
/** Generic CPUID validation error. */
#define GIM_HV_STATUS_CPUID_FEAT_VALIDATION_ERROR                    0x3C
/** XSAVE CPUID validation error. */
#define GIM_HV_STATUS_CPUID_XSAVE_FEAT_VALIDATION_ERROR              0x3D
/** Processor startup timed out. */
#define GIM_HV_STATUS_PROCESSOR_STARTUP_TIMEOUT                      0x3E
/** SMX enabled by the BIOS. */
#define GIM_HV_STATUS_SMX_ENABLED                                    0x3F
/** Processor feature PCID unsupported. */
#define GIM_HV_STATUS_PROC_FEAT_PCID_NOT_SUPPORTED                   0x40
/** Invalid LP index. */
#define GIM_HV_STATUS_INVALID_LP_INDEX                               0x41
/** Processor feature PCID unsupported. */
#define GIM_HV_STATUS_FEAT_FMA4_NOT_SUPPORTED                        0x42
/** Processor feature PCID unsupported. */
#define GIM_HV_STATUS_FEAT_F16C_NOT_SUPPORTED                        0x43
/** Processor feature PCID unsupported. */
#define GIM_HV_STATUS_PROC_FEAT_RDRAND_NOT_SUPPORTED                 0x44
/** Processor feature RDWRFSGS unsupported. */
#define GIM_HV_STATUS_PROC_FEAT_RDWRFSGS_NOT_SUPPORTED               0x45
/** Processor feature SMEP unsupported. */
#define GIM_HV_STATUS_PROC_FEAT_SMEP_NOT_SUPPORTED                   0x46
/** Processor feature enhanced fast string unsupported. */
#define GIM_HV_STATUS_PROC_FEAT_ENHANCED_FAST_STRING_NOT_SUPPORTED   0x47
/** Processor feature MOVBE unsupported. */
#define GIM_HV_STATUS_PROC_FEAT_MOVBE_NOT_SUPPORTED                  0x48
/** Processor feature BMI1 unsupported. */
#define GIM_HV_STATUS_PROC_FEAT_BMI1_NOT_SUPPORTED                   0x49
/** Processor feature BMI2 unsupported. */
#define GIM_HV_STATUS_PROC_FEAT_BMI2_NOT_SUPPORTED                   0x4A
/** Processor feature HLE unsupported. */
#define GIM_HV_STATUS_PROC_FEAT_HLE_NOT_SUPPORTED                    0x4B
/** Processor feature RTM unsupported. */
#define GIM_HV_STATUS_PROC_FEAT_RTM_NOT_SUPPORTED                    0x4C
/** Processor feature XSAVE FMA unsupported. */
#define GIM_HV_STATUS_PROC_FEAT_XSAVE_FMA_NOT_SUPPORTED              0x4D
/** Processor feature XSAVE AVX2 unsupported. */
#define GIM_HV_STATUS_PROC_FEAT_XSAVE_AVX2_NOT_SUPPORTED             0x4E
/** Processor feature NPIEP1 unsupported. */
#define GIM_HV_STATUS_PROC_FEAT_NPIEP1_NOT_SUPPORTED                 0x4F
/** @} */


/** @name Hyper-V MSR - Debug control (MSR_GIM_HV_SYNTH_DEBUG_CONTROL).
 * @{
 */
/** Perform debug write. */
#define MSR_GIM_HV_SYNTH_DEBUG_CONTROL_IS_WRITE(a)     RT_BOOL((a) & RT_BIT_64(0))
/** Perform debug read. */
#define MSR_GIM_HV_SYNTH_DEBUG_CONTROL_IS_READ(a)      RT_BOOL((a) & RT_BIT_64(1))
/** Returns length of the debug write buffer. */
#define MSR_GIM_HV_SYNTH_DEBUG_CONTROL_W_LEN(a)        (((a) & UINT64_C(0xffff0000)) >> 16)
/** @} */


/** @name Hyper-V MSR - Debug status (MSR_GIM_HV_SYNTH_DEBUG_STATUS).
 * @{
 */
/** Debug send buffer operation success. */
#define MSR_GIM_HV_SYNTH_DEBUG_STATUS_W_SUCCESS_BIT    RT_BIT_32(0)
/** Debug receive buffer operation success. */
#define MSR_GIM_HV_SYNTH_DEBUG_STATUS_R_SUCCESS_BIT    RT_BIT_32(2)
/** Debug connection was reset. */
#define MSR_GIM_HV_SYNTH_DEBUG_STATUS_CONN_RESET_BIT   RT_BIT_32(3)
/** @} */


/** @name Hyper-V hypercall debug support.
 * Options and constants for Hyper-V debug hypercalls.
 * @{
 */
/** Maximum debug data payload size in bytes. */
#define GIM_HV_DEBUG_MAX_DATA_SIZE                4088

/** The undocumented bit for MSR_GIM_HV_DEBUG_OPTIONS_MSR that makes it all
 *  work. */
#define GIM_HV_DEBUG_OPTIONS_MSR_ENABLE           RT_BIT(2)

/** Guest will perform the HvPostDebugData hypercall until completion. */
#define GIM_HV_DEBUG_POST_LOOP                    RT_BIT_32(0)
/** Mask of valid HvPostDebugData options. */
#define GIM_HV_DEBUG_POST_OPTIONS_MASK            RT_BIT_32(0)

/** Guest will perform the HvRetrieveDebugData hypercall until completion. */
#define GIM_HV_DEBUG_RETREIVE_LOOP                RT_BIT_32(0)
/** Guest checks if any global debug session is active. */
#define GIM_HV_DEBUG_RETREIVE_TEST_ACTIVITY       RT_BIT_32(1)
/** Mask of valid HvRetrieveDebugData options. */
#define GIM_HV_DEBUG_RETREIVE_OPTIONS_MASK        RT_BIT_32(0) | RT_BIT_32(1)

/** Guest requests purging of incoming debug data. */
#define GIM_HV_DEBUG_PURGE_INCOMING_DATA          RT_BIT_32(0)
/** Guest requests purging of outgoing debug data. */
#define GIM_HV_DEBUG_PURGE_OUTGOING_DATA          RT_BIT_32(1)

/**
 * HvResetDebugData hypercall input.
 */
typedef struct GIMHVDEBUGRESETIN
{
    uint32_t fFlags;
    uint32_t uPadding;
} GIMHVDEBUGRESETIN;
/** Pointer to a HvResetDebugData input struct. */
typedef GIMHVDEBUGRESETIN *PGIMHVDEBUGRESETIN;
AssertCompileSize(GIMHVDEBUGRESETIN, 8);

/**
 * HvPostDebugData hypercall input.
 */
typedef struct GIMHVDEBUGPOSTIN
{
    uint32_t cbWrite;
    uint32_t fFlags;
} GIMHVDEBUGPOSTIN;
/** Pointer to a HvPostDebugData input struct. */
typedef GIMHVDEBUGPOSTIN *PGIMHVDEBUGPOSTIN;
AssertCompileSize(GIMHVDEBUGPOSTIN, 8);

/**
 * HvPostDebugData hypercall output.
 */
typedef struct GIMHVDEBUGPOSTOUT
{
    uint32_t cbPending;
    uint32_t uPadding;
} GIMHVDEBUGPOSTOUT;
/** Pointer to a HvPostDebugData output struct. */
typedef GIMHVDEBUGPOSTOUT *PGIMHVDEBUGPOSTOUT;
AssertCompileSize(GIMHVDEBUGPOSTOUT, 8);

/**
 * HvRetrieveDebugData hypercall input.
 */
typedef struct GIMHVDEBUGRETRIEVEIN
{
    uint32_t cbRead;
    uint32_t fFlags;
    uint64_t u64Timeout;
} GIMHVDEBUGRETRIEVEIN;
/** Pointer to a HvRetrieveDebugData input struct. */
typedef GIMHVDEBUGRETRIEVEIN *PGIMHVDEBUGRETRIEVEIN;
AssertCompileSize(GIMHVDEBUGRETRIEVEIN, 16);

/**
 * HvRetriveDebugData hypercall output.
 */
typedef struct GIMHVDEBUGRETRIEVEOUT
{
    uint32_t cbRead;
    uint32_t cbRemaining;
} GIMHVDEBUGRETRIEVEOUT;
/** Pointer to a HvRetrieveDebugData output struct. */
typedef GIMHVDEBUGRETRIEVEOUT *PGIMHVDEBUGRETRIEVEOUT;
AssertCompileSize(GIMHVDEBUGRETRIEVEOUT, 8);
/** @} */


/** Hyper-V page size.  */
#define GIM_HV_PAGE_SIZE                          4096

/** Microsoft Hyper-V vendor signature. */
#define GIM_HV_VENDOR_MICROSOFT                   "Microsoft Hv"

/**
 * MMIO2 region indices.
 */
/** The hypercall page region. */
#define GIM_HV_HYPERCALL_PAGE_REGION_IDX          UINT8_C(0)
/** The TSC page region. */
#define GIM_HV_REF_TSC_PAGE_REGION_IDX            UINT8_C(1)
/** The maximum region index (must be <= UINT8_MAX). */
#define GIM_HV_REGION_IDX_MAX                     GIM_HV_REF_TSC_PAGE_REGION_IDX

/**
 * Hyper-V TSC (HV_REFERENCE_TSC_PAGE) structure placed in the TSC reference
 * page.
 */
typedef struct GIMHVREFTSC
{
    uint32_t u32TscSequence;
    uint32_t uReserved0;
    uint64_t u64TscScale;
    int64_t  i64TscOffset;
} GIMHVTSCPAGE;
/** Pointer to Hyper-V reference TSC. */
typedef GIMHVREFTSC *PGIMHVREFTSC;
/** Pointer to a const Hyper-V reference TSC. */
typedef GIMHVREFTSC const *PCGIMHVREFTSC;

/**
 * GIM Hyper-V VM instance data.
 * Changes to this must checked against the padding of the gim union in VM!
 */
typedef struct GIMHV
{
    /** @name Primary MSRs.
     * @{ */
    /** Guest OS identity MSR. */
    uint64_t                    u64GuestOsIdMsr;
    /** Hypercall MSR. */
    uint64_t                    u64HypercallMsr;
    /** Reference TSC page MSR. */
    uint64_t                    u64TscPageMsr;
    /** @}  */

    /** @name CPUID features.
     * @{ */
    /** Basic features. */
    uint32_t                    uBaseFeat;
    /** Partition flags. */
    uint32_t                    uPartFlags;
    /** Power management. */
    uint32_t                    uPowMgmtFeat;
    /** Miscellaneous. */
    uint32_t                    uMiscFeat;
    /** Hypervisor hints to the guest. */
    uint32_t                    uHyperHints;
    /** Hypervisor capabilities. */
    uint32_t                    uHyperCaps;
    /** @} */

    /** @name Guest Crash MSRs.
     * @{
     */
    /** Guest crash control MSR. */
    uint64_t                    uCrashCtl;
    /** Guest crash parameter 0 MSR. */
    uint64_t                    uCrashP0;
    /** Guest crash parameter 1 MSR. */
    uint64_t                    uCrashP1;
    /** Guest crash parameter 2 MSR. */
    uint64_t                    uCrashP2;
    /** Guest crash parameter 3 MSR. */
    uint64_t                    uCrashP3;
    /** Guest crash parameter 4 MSR. */
    uint64_t                    uCrashP4;
    /** @} */

    /** @name Time management.
     * @{ */
    /** Per-VM R0 Spinlock for protecting EMT writes to the TSC page. */
    RTSPINLOCK                  hSpinlockR0;
#if HC_ARCH_BITS == 32
    uint32_t                    u32Alignment1;
#endif
    /** The TSC frequency (in HZ) reported to the guest. */
    uint64_t                    cTscTicksPerSecond;
    /** @} */

    /** @name Hypercalls. */
    /* @{ */
    /** Pointer to the hypercall input parameter page - R3. */
    R3PTRTYPE(uint8_t *)        pbHypercallIn;
    /** Pointer to the hypercall output parameter page - R3. */
    R3PTRTYPE(uint8_t *)        pbHypercallOut;
    /** @} */

    /** @name Guest debugging.
     * @{ */
    /** Whether we're posing as the Microsoft vendor. */
    bool                        fIsVendorMsHv;
    /** Whether we're posing as the Microsoft virtualization service. */
    bool                        fIsInterfaceVs;
    bool                        afAlignment0[6];
    /** The auto IP address last chosen by the guest after failed ARP queries. */
    RTNETADDRIPV4               DbgGuestAddr;
    uint32_t                    uAlignment1;
    /** Debug send buffer MSR. */
    uint64_t                    uDebugSendBufferMsr;
    /** Debug receive buffer MSR. */
    uint64_t                    uDebugRecvBufferMsr;
    /** Debug pending buffer MSR. */
    uint64_t                    uDebugPendingBufferMsr;
    /** Debug status MSR. */
    uint64_t                    uDebugStatusMsr;
    /** @} */

    /** Array of MMIO2 regions. */
    GIMMMIO2REGION              aMmio2Regions[GIM_HV_REGION_IDX_MAX + 1];
} GIMHV;
/** Pointer to per-VM GIM Hyper-V instance data. */
typedef GIMHV *PGIMHV;
/** Pointer to const per-VM GIM Hyper-V instance data. */
typedef GIMHV const *PCGIMHV;
AssertCompileMemberAlignment(GIMHV, aMmio2Regions, 8);
AssertCompileMemberAlignment(GIMHV, hSpinlockR0, sizeof(uintptr_t));

RT_C_DECLS_BEGIN

#ifdef IN_RING0
VMMR0_INT_DECL(int)             gimR0HvInitVM(PVM pVM);
VMMR0_INT_DECL(int)             gimR0HvTermVM(PVM pVM);
VMMR0_INT_DECL(int)             gimR0HvUpdateParavirtTsc(PVM pVM, uint64_t u64Offset);
#endif /* IN_RING0 */

#ifdef IN_RING3
VMMR3_INT_DECL(int)             gimR3HvInit(PVM pVM, PCFGMNODE pGimCfg);
VMMR3_INT_DECL(int)             gimR3HvInitCompleted(PVM pVM);
VMMR3_INT_DECL(int)             gimR3HvTerm(PVM pVM);
VMMR3_INT_DECL(void)            gimR3HvRelocate(PVM pVM, RTGCINTPTR offDelta);
VMMR3_INT_DECL(void)            gimR3HvReset(PVM pVM);
VMMR3_INT_DECL(PGIMMMIO2REGION) gimR3HvGetMmio2Regions(PVM pVM, uint32_t *pcRegions);
VMMR3_INT_DECL(int)             gimR3HvSave(PVM pVM, PSSMHANDLE pSSM);
VMMR3_INT_DECL(int)             gimR3HvLoad(PVM pVM, PSSMHANDLE pSSM, uint32_t uSSMVersion);

VMMR3_INT_DECL(int)             gimR3HvDisableTscPage(PVM pVM);
VMMR3_INT_DECL(int)             gimR3HvEnableTscPage(PVM pVM, RTGCPHYS GCPhysTscPage, bool fUseThisTscSeq, uint32_t uTscSeq);
VMMR3_INT_DECL(int)             gimR3HvDisableHypercallPage(PVM pVM);
VMMR3_INT_DECL(int)             gimR3HvEnableHypercallPage(PVM pVM, RTGCPHYS GCPhysHypercallPage);

VMMR3_INT_DECL(int)             gimR3HvHypercallPostDebugData(PVM pVM, RTGCPHYS GCPhysOut, int *prcHv);
VMMR3_INT_DECL(int)             gimR3HvHypercallRetrieveDebugData(PVM pVM, RTGCPHYS GCPhysOut, int *prcHv);
VMMR3_INT_DECL(int)             gimR3HvDebugWrite(PVM pVM, void *pvData, uint32_t cbWrite, uint32_t *pcbWritten, bool fUdpPkt);
VMMR3_INT_DECL(int)             gimR3HvDebugRead(PVM pVM, void *pvBuf, uint32_t cbBuf, uint32_t cbRead, uint32_t *pcbRead,
                                                 uint32_t cMsTimeout, bool fUdpPkt);
#endif /* IN_RING3 */

VMM_INT_DECL(bool)              gimHvIsParavirtTscEnabled(PVM pVM);
VMM_INT_DECL(bool)              gimHvAreHypercallsEnabled(PVMCPU pVCpu);
VMM_INT_DECL(bool)              gimHvShouldTrapXcptUD(PVMCPU pVCpu);
VMM_INT_DECL(int)               gimHvXcptUD(PVMCPU pVCpu, PCPUMCTX pCtx, PDISCPUSTATE pDis);
VMM_INT_DECL(int)               gimHvHypercall(PVMCPU pVCpu, PCPUMCTX pCtx);
VMM_INT_DECL(VBOXSTRICTRC)      gimHvReadMsr(PVMCPU pVCpu, uint32_t idMsr, PCCPUMMSRRANGE pRange, uint64_t *puValue);
VMM_INT_DECL(VBOXSTRICTRC)      gimHvWriteMsr(PVMCPU pVCpu, uint32_t idMsr, PCCPUMMSRRANGE pRange, uint64_t uRawValue);

RT_C_DECLS_END

#endif

