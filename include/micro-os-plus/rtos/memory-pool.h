/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2016 Liviu Ionescu.
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can
 * be obtained from https://opensource.org/licenses/MIT/.
 */

#ifndef MICRO_OS_PLUS_RTOS_MEMORY_POOL_H_
#define MICRO_OS_PLUS_RTOS_MEMORY_POOL_H_

// ----------------------------------------------------------------------------

#if defined(__cplusplus)

// ----------------------------------------------------------------------------

#include <micro-os-plus/rtos/declarations.h>
#include <micro-os-plus/rtos/memory.h>

#include <micro-os-plus/diag/trace.h>

// ----------------------------------------------------------------------------

#pragma GCC diagnostic push

#if defined(__clang__)
#pragma clang diagnostic ignored "-Wc++98-compat"
#endif
#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic ignored "-Wsuggest-final-methods"
#pragma GCC diagnostic ignored "-Wsuggest-final-types"
#endif

namespace micro_os_plus
{
  namespace rtos
  {

    // ========================================================================

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

    /**
     * @brief Synchronised **memory pool**, using the
     * default RTOS allocator.
     * @headerfile os.h <micro-os-plus/rtos.h>
     * @ingroup micro-os-plus-rtos-mempool
     */
    class memory_pool : public internal::object_named_system
    {
    public:
      // ======================================================================

      /**
       * @brief Type of memory pool size storage.
       * @details
       * A numeric value that can hold the maximum size of the
       * memory pool, usually a 16-bits unsigned value.
       * @ingroup micro-os-plus-rtos-mempool
       */
      using size_t = uint16_t;

      /**
       * @brief Maximum pool size.
       * @details
       * A constant numeric value used to validate the pool size.
       * @ingroup micro-os-plus-rtos-mempool
       */
      static constexpr memory_pool::size_t max_size
          = static_cast<memory_pool::size_t> (0 - 1);

      // ======================================================================

      /**
       * @brief Memory pool attributes.
       * @headerfile os.h <micro-os-plus/rtos.h>
       * @ingroup micro-os-plus-rtos-mempool
       */
      class attributes : public internal::attributes_clocked
      {
      public:
        /**
         * @name Constructors & Destructor
         * @{
         */

        /**
         * @brief Construct a memory pool attributes object instance.
         * @par Parameters
         *  None.
         */
        constexpr attributes ();

        // The rule of five.
        attributes (const attributes&) = default;
        attributes (attributes&&) = default;
        attributes&
        operator= (const attributes&)
            = default;
        attributes&
        operator= (attributes&&)
            = default;

        /**
         * @brief Destruct the memory pool attributes object instance.
         */
        ~attributes () = default;

        /**
         * @}
         */

      public:
        /**
         * @name Public Member Variables
         * @{
         */

        // Public members; no accessors and mutators required.
        // Warning: must match the type & order of the C file header.
        /**
         * @brief Address of the user defined storage for the memory pool.
         */
        void* arena_address = nullptr;

        /**
         * @brief Size of the user defined storage for the memory pool.
         */
        std::size_t arena_size_bytes = 0;

        // Add more attributes here.

        /**
         * @}
         */

      }; /* class attributes */

      /**
       * @brief Default memory pool initialiser.
       * @ingroup micro-os-plus-rtos-mempool
       */
      static const attributes initializer;

      /**
       * @brief Storage for a memory pool.
       * @details
       * Each message is stored in an element
       * extended to a multiple of pointers. The free lists
       * is kept inside the blocks and does not require additional
       * storage.
       */
      template <typename T, std::size_t blocks, std::size_t block_size_bytes>
      class arena
      {
      public:
        T pool[(blocks * block_size_bytes + sizeof (T) - 1) / sizeof (T)];
      };

      /**
       * @brief Calculator for pool storage requirements.
       * @param blocks Number of blocks.
       * @param block_size_bytes Size of block.
       * @return Total required storage in bytes, including internal alignment.
       */
      template <typename T>
      constexpr std::size_t
      compute_allocated_size_bytes (std::size_t blocks,
                                    std::size_t block_size_bytes)
      {
        // Align each block
        return (blocks
                * ((block_size_bytes + (sizeof (T) - 1)) & ~(sizeof (T) - 1)));
      }

      // ======================================================================

      /**
       * @brief Default RTOS allocator.
       */
      using allocator_type
          = memory::allocator<thread::stack::allocation_element_t>;

      /**
       * @name Constructors & Destructor
       * @{
       */

      /**
       * @brief Construct a memory pool object instance.
       * @param [in] blocks The maximum number of items in the pool.
       * @param [in] block_size_bytes The size of an item, in bytes.
       * @param [in] attributes Reference to attributes.
       * @param [in] allocator Reference to allocator. Default a
       * local temporary instance.
       */
      memory_pool (std::size_t blocks, std::size_t block_size_bytes,
                   const attributes& attributes = initializer,
                   const allocator_type& allocator = allocator_type ());
      /**
       * @brief Construct a named memory pool object instance.
       * @param [in] name Pointer to name.
       * @param [in] blocks The maximum number of items in the pool.
       * @param [in] block_size_bytes The size of an item, in bytes.
       * @param [in] attributes Reference to attributes.
       * @param [in] allocator Reference to allocator. Default a
       * local temporary instance.
       */
      memory_pool (const char* name, std::size_t blocks,
                   std::size_t block_size_bytes,
                   const attributes& attributes = initializer,
                   const allocator_type& allocator = allocator_type ());

    protected:
      /**
       * @cond ignore
       */

      // Internal constructors, used from templates.
      memory_pool ();
      memory_pool (const char* name);

      /**
       * @endcond
       */

    public:
      /**
       * @cond ignore
       */

      // The rule of five.
      memory_pool (const memory_pool&) = delete;
      memory_pool (memory_pool&&) = delete;
      memory_pool&
      operator= (const memory_pool&)
          = delete;
      memory_pool&
      operator= (memory_pool&&)
          = delete;

      /**
       * @endcond
       */

    public:
      /**
       * @brief Destruct the memory pool object instance.
       */
      virtual ~memory_pool ();

      /**
       * @}
       */

      /**
       * @name Operators
       * @{
       */

      /**
       * @brief Compare memory pools.
       * @retval true The given memory pool is the same as this memory pool.
       * @retval false The memory pools are different.
       */
      bool
      operator== (const memory_pool& rhs) const;

      /**
       * @}
       */

    public:
      /**
       * @name Public Member Functions
       * @{
       */

      /**
       * @brief Allocate a memory block.
       * @par Parameters
       *  None.
       * @return Pointer to memory block, or `nullptr` if interrupted.
       */
      void*
      alloc (void);

      /**
       * @brief Try to allocate a memory block.
       * @par Parameters
       *  None.
       * @return Pointer to memory block, or `nullptr` if no memory available.
       */
      void*
      try_alloc (void);

      /**
       * @brief Allocate a memory block with timeout.
       * @param [in] timeout Timeout to wait, in clock units (ticks or
       * seconds).
       * @return Pointer to memory block, or `nullptr` if timeout.
       */
      void*
      timed_alloc (clock::duration_t timeout);

      /**
       * @brief Free the memory block.
       * @param [in] block Pointer to memory block to free.
       * @retval result::ok The memory block was released.
       * @retval EINVAL The block does not belong to the memory pool.
       */
      result_t
      free (void* block);

      /**
       * @brief Get memory pool capacity.
       * @par Parameters
       *  None.
       * @return The max number of blocks in the pool.
       */
      std::size_t
      capacity (void) const;

      /**
       * @brief Get blocks count.
       * @par Parameters
       *  None.
       * @return The number of blocks used from the queue.
       */
      std::size_t
      count (void) const;

      /**
       * @brief Get block size.
       * @par Parameters
       *  None.
       * @return The block size, in bytes.
       */
      std::size_t
      block_size (void) const;

      /**
       * @brief Check if the memory pool is empty.
       * @par Parameters
       *  None
       * @retval true The memory pool has no allocated blocks.
       * @retval false The memory pool has allocated blocks.
       */
      bool
      empty (void) const;

      /**
       * @brief Check if the memory pool is full.
       * @par Parameters
       *  None.
       * @retval true All memory blocks are allocated.
       * @retval false There are still memory blocks that can be allocated.
       */
      bool
      full (void) const;

      /**
       * @brief Reset the memory pool.
       * @par Parameters
       *  None.
       * @retval result::ok The memory pool was reset.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
       */
      result_t
      reset (void);

      /**
       * @brief Get the pool storage address.
       * @par Parameters
       *  None.
       * @return Pointer to storage.
       */
      void*
      pool (void);

      /**
       * @}
       */

    protected:
      /**
       * @name Private Member Functions
       * @{
       */

      /**
       * @cond ignore
       */

      /**
       * @brief Internal function used during memory pool construction.
       * @param [in] blocks The maximum number of items in the pool.
       * @param [in] block_size_bytes The size of an item, in bytes.
       * @param [in] attributes Reference to attributes.
       * @param [in] arena_address Pointer to queue storage.
       * @param [in] arena_size_bytes Size of queue storage.
       */
      void
      internal_construct_ (std::size_t blocks, std::size_t block_size_bytes,
                           const attributes& attributes, void* arena_address,
                           std::size_t arena_size_bytes);

      /**
       * @brief Internal initialisation.
       * @par Parameters
       *  None.
       * @par Returns
       *  Nothing.
       */
      void
      internal_init_ (void);

      /**
       * @brief Internal function used to get the first linked block.
       * @par Parameters
       *  None.
       * @return Pointer to block or `nullptr` if no more blocks available.
       */
      void*
      internal_try_first_ (void);

      /**
       * @endcond
       */

      /**
       * @}
       */

    protected:
      /**
       * @name Private Member Variables
       * @{
       */

      /**
       * @cond ignore
       */

#if !defined(MICRO_OS_PLUS_USE_RTOS_PORT_MEMORY_POOL)
      /**
       * @brief List of threads waiting to alloc.
       */
      internal::waiting_threads_list list_;
      /**
       * @brief Pointer to clock to be used for timeouts.
       */
      clock* clock_ = nullptr;
#endif
      /**
       * @brief The static address where the pool is stored
       * (from `attributes.arena_address`).
       */
      void* pool_arena_address_ = nullptr;
      /**
       * @brief The dynamic address if the pool was allocated
       * (and must be deallocated)
       */
      void* allocated_pool_arena_address_ = nullptr;

      /**
       * @brief Pointer to allocator.
       */
      const void* allocator_ = nullptr;

#if defined(MICRO_OS_PLUS_USE_RTOS_PORT_MEMORY_POOL)
      friend class port::memory_pool;
      micro_os_plus_memory_pool_port_data_t port_;
#endif

      /**
       * @brief Total size of the statically allocated pool storage
       * (from `attributes.arena_size_bytes`).
       */
      std::size_t pool_arena_size_bytes_ = 0;
      /**
       * @brief Total size of the dynamically allocated pool storage.
       */
      std::size_t allocated_pool_size_elements_ = 0;

      /**
       * @brief The number of blocks in the pool.
       */
      memory_pool::size_t blocks_ = 0;

      /**
       * @brief The size of a block, in bytes.
       */
      memory_pool::size_t block_size_bytes_ = 0;

      /**
       * @brief The current number of blocks allocated from the pool.
       */
      volatile memory_pool::size_t count_ = 0;

      /**
       * @brief Pointer to the first free block, or nullptr.
       */
      void* volatile first_ = nullptr;

      /**
       * @endcond
       */

      /**
       * @}
       */
    };

    // ========================================================================

    /**
     * @brief Template of a synchronised **memory pool** with allocator.
     * @headerfile os.h <micro-os-plus/rtos.h>
     * @ingroup micro-os-plus-rtos-mempool
     */
    template <typename Allocator = memory::allocator<void*>>
    class memory_pool_allocated : public memory_pool
    {
    public:
      /**
       * @brief Standard allocator type definition.
       */
      using allocator_type = Allocator;

      /**
       * @name Constructors & Destructor
       * @{
       */

      /**
       * @brief Construct a memory pool object instance.
       * @param [in] blocks The maximum number of items in the pool.
       * @param [in] block_size_bytes The size of an item, in bytes.
       * @param [in] attributes Reference to attributes.
       * @param [in] allocator Reference to allocator. Default a
       * local temporary instance.
       */
      memory_pool_allocated (std::size_t blocks, std::size_t block_size_bytes,
                             const attributes& attributes = initializer,
                             const allocator_type& allocator
                             = allocator_type ());

      /**
       * @brief Construct a named memory pool object instance.
       * @param [in] name Pointer to name.
       * @param [in] blocks The maximum number of items in the pool.
       * @param [in] block_size_bytes The size of an item, in bytes.
       * @param [in] attributes Reference to attributes.
       * @param [in] allocator Reference to allocator. Default a
       * local temporary instance.
       */
      memory_pool_allocated (const char* name, std::size_t blocks,
                             std::size_t block_size_bytes,
                             const attributes& attributes = initializer,
                             const allocator_type& allocator
                             = allocator_type ());

      /**
       * @cond ignore
       */

      // The rule of five.
      memory_pool_allocated (const memory_pool_allocated&) = delete;
      memory_pool_allocated (memory_pool_allocated&&) = delete;
      memory_pool_allocated&
      operator= (const memory_pool_allocated&)
          = delete;
      memory_pool_allocated&
      operator= (memory_pool_allocated&&)
          = delete;

      /**
       * @endcond
       */

      /**
       * @brief Destruct the memory pool object instance.
       */
      virtual ~memory_pool_allocated ();

      /**
       * @}
       */
    };

    // ========================================================================

    /**
     * @brief Template of a synchronised **memory pool** with
     * block type and allocator.
     * @headerfile os.h <micro-os-plus/rtos.h>
     * @ingroup micro-os-plus-rtos-mempool
     */
    template <typename T, typename Allocator = memory::allocator<void*>>
    class memory_pool_typed : public memory_pool_allocated<Allocator>
    {
    public:
      /**
       * @brief Standard allocator type definition.
       */
      using value_type = T;
      /**
       * @brief Standard allocator type definition.
       */
      using allocator_type = Allocator;

      /**
       * @name Constructors & Destructor
       * @{
       */

      /**
       * @brief Construct a memory pool object instance.
       * @param [in] blocks The maximum number of items in the pool.
       * @param [in] attributes Reference to attributes.
       * @param [in] allocator Reference to allocator. Default a
       * local temporary instance.
       */
      memory_pool_typed (std::size_t blocks,
                         const memory_pool::attributes& attributes
                         = memory_pool::initializer,
                         const allocator_type& allocator = allocator_type ());

      /**
       * @brief Construct a named memory pool object instance.
       * @param [in] name Pointer to name.
       * @param [in] blocks The maximum number of items in the pool.
       * @param [in] attributes Reference to attributes.
       * @param [in] allocator Reference to allocator. Default a
       * local temporary instance.
       */
      memory_pool_typed (const char* name, std::size_t blocks,
                         const memory_pool::attributes& attributes
                         = memory_pool::initializer,
                         const allocator_type& allocator = allocator_type ());

      /**
       * @cond ignore
       */

      // The rule of five.
      memory_pool_typed (const memory_pool_typed&) = delete;
      memory_pool_typed (memory_pool_typed&&) = delete;
      memory_pool_typed&
      operator= (const memory_pool_typed&)
          = delete;
      memory_pool_typed&
      operator= (memory_pool_typed&&)
          = delete;

      /**
       * @endcond
       */

      /**
       * @brief Destruct the memory pool object instance.
       */
      virtual ~memory_pool_typed ();

      /**
       * @}
       */

    public:
      /**
       * @name Public Member Functions
       * @{
       */

      /**
       * @brief Allocate a memory block.
       * @par Parameters
       *  None.
       * @return Pointer to memory block, or `nullptr` if interrupted.
       */
      value_type*
      alloc (void);

      /**
       * @brief Allocate a memory block.
       * @par Parameters
       *  None.
       * @return Pointer to memory block, or `nullptr` if no memory available.
       */
      value_type*
      try_alloc (void);

      /**
       * @brief Allocate a memory block with timeout.
       * @param [in] timeout Timeout to wait, in clock units (ticks or
       * seconds).
       * @return Pointer to memory block, or `nullptr` if timeout.
       */
      value_type*
      timed_alloc (clock::duration_t timeout);

      /**
       * @brief Free the memory block.
       * @par Parameters
       *  None.
       * @retval result::ok The memory block was released.
       * @retval EINVAL The block does not belong to the memory pool.
       */
      result_t
      free (value_type* block);

      /**
       * @}
       */
    };

    // ========================================================================

    /**
     * @brief Template of a synchronised **memory pool** with
     * block type and local storage.
     * @headerfile os.h <micro-os-plus/rtos.h>
     * @ingroup micro-os-plus-rtos-mempool
     */
    template <typename T, std::size_t N>
    class memory_pool_inclusive : public memory_pool
    {
    public:
      /**
       * @brief Local type of message.
       */
      using value_type = T;

      /**
       * @brief Local constant based on template definition.
       */
      static const std::size_t blocks = N;

      /**
       * @name Constructors & Destructor
       * @{
       */

      /**
       * @brief Construct a memory pool object instance.
       * @param [in] attributes Reference to attributes.
       */
      memory_pool_inclusive (const attributes& attributes = initializer);

      /**
       * @brief Construct a named memory pool object instance.
       * @param [in] name Pointer to name.
       * @param [in] attributes Reference to attributes.
       */
      memory_pool_inclusive (const char* name,
                             const attributes& attributes = initializer);

      /**
       * @cond ignore
       */

      // The rule of five.
      memory_pool_inclusive (const memory_pool_inclusive&) = delete;
      memory_pool_inclusive (memory_pool_inclusive&&) = delete;
      memory_pool_inclusive&
      operator= (const memory_pool_inclusive&)
          = delete;
      memory_pool_inclusive&
      operator= (memory_pool_inclusive&&)
          = delete;

      /**
       * @endcond
       */

      /**
       * @brief Destruct the memory pool object instance.
       */
      virtual ~memory_pool_inclusive ();

      /**
       * @}
       */

    protected:
      /**
       * @name Private Member Variables
       * @{
       */

      /**
       * @brief Local storage for the pool.
       * @details
       * The local storage is large enough to include `blocks`
       * blocks of type `T`.
       * For performance reasons, the individual components are
       * aligned as pointers.
       */
      arena<void*, blocks, sizeof (T)> arena_;

      /**
       * @}
       */

    public:
      /**
       * @name Public Member Functions
       * @{
       */

      /**
       * @brief Allocate a memory block.
       * @par Parameters
       *  None.
       * @return Pointer to memory block, or `nullptr` if interrupted.
       */
      value_type*
      alloc (void);

      /**
       * @brief Allocate a memory block.
       * @par Parameters
       *  None.
       * @return Pointer to memory block, or `nullptr` if no memory available.
       */
      value_type*
      try_alloc (void);

      /**
       * @brief Allocate a memory block with timeout.
       * @param [in] timeout Timeout to wait, in clock units (ticks or
       * seconds).
       * @return Pointer to memory block, or `nullptr` if timeout.
       */
      value_type*
      timed_alloc (clock::duration_t timeout);

      /**
       * @brief Free the memory block.
       * @par Parameters
       *  None.
       * @retval result::ok The memory block was released.
       * @retval EINVAL The block does not belong to the memory pool.
       */
      result_t
      free (value_type* block);

      /**
       * @}
       */
    };

#pragma GCC diagnostic pop

  } // namespace rtos
} // namespace micro_os_plus

// ===== Inline & template implementations ====================================

namespace micro_os_plus
{
  namespace rtos
  {
    constexpr memory_pool::attributes::attributes ()
    {
      ;
    }

    // ========================================================================

    /**
     * @details
     * Identical memory pools should have the same memory address.
     */
    inline bool
    memory_pool::operator== (const memory_pool& rhs) const
    {
      return this == &rhs;
    }

    /**
     * @note Can be invoked from Interrupt Service Routines.
     */
    inline std::size_t
    memory_pool::capacity (void) const
    {
      return blocks_;
    }

    /**
     * @note Can be invoked from Interrupt Service Routines.
     */
    inline std::size_t
    memory_pool::block_size (void) const
    {
      return block_size_bytes_;
    }

    /**
     * @note Can be invoked from Interrupt Service Routines.
     */
    inline std::size_t
    memory_pool::count (void) const
    {
      return count_;
    }

    /**
     * @note Can be invoked from Interrupt Service Routines.
     */
    inline bool
    memory_pool::empty (void) const
    {
      return (count () == 0);
    }

    /**
     * @note Can be invoked from Interrupt Service Routines.
     */
    inline bool
    memory_pool::full (void) const
    {
      return (count () == capacity ());
    }

    /**
     * @note Can be invoked from Interrupt Service Routines.
     */
    inline void*
    memory_pool::pool (void)
    {
      return pool_arena_address_;
    }

    // ========================================================================

    /**
     * @details
     * This constructor shall initialise a memory pool object
     * with attributes referenced by _attr_.
     * If the attributes specified by _attr_ are modified later,
     * the memory pool attributes shall not be affected.
     * Upon successful initialisation, the state of the memory pool
     * variable shall become initialised.
     *
     * Only the memory pool itself may be used for allocations.
     * It is not allowed to make copies of
     * condition variable objects.
     *
     * In cases where default memory pool attributes are
     * appropriate, the variable `memory_pool::initializer` can be used to
     * initialise condition variables.
     * The effect shall be equivalent to creating a memory pool object with
     * the simple constructor.
     *
     * If the attributes define a storage area (via `arena_address`
     * and `arena_size_bytes`), that storage is used, otherwise the
     * storage is dynamically allocated using the RTOS specific allocator
     * (`rtos::memory::allocator`).
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    template <typename Allocator>
    inline memory_pool_allocated<Allocator>::memory_pool_allocated (
        std::size_t blocks, std::size_t block_size_bytes,
        const attributes& _attributes, const allocator_type& allocator)
        : memory_pool_allocated{ nullptr, blocks, block_size_bytes,
                                 _attributes, allocator }
    {
      ;
    }

    /**
     * @details
     * This constructor shall initialise a named memory pool object
     * with attributes referenced by _attr_.
     * If the attributes specified by _attr_ are modified later,
     * the memory pool attributes shall not be affected.
     * Upon successful initialisation, the state of the memory pool
     * variable shall become initialised.
     *
     * Only the memory pool itself may be used for allocations.
     * It is not allowed to make copies of
     * condition variable objects.
     *
     * In cases where default memory pool attributes are
     * appropriate, the variable `memory_pool::initializer` can be used to
     * initialise condition variables.
     * The effect shall be equivalent to creating a memory pool object with
     * the simple constructor.
     *
     * If the attributes define a storage area (via `arena_address`
     * and `arena_size_bytes`), that storage is used, otherwise the
     * storage is dynamically allocated using the RTOS specific allocator
     * (`rtos::memory::allocator`).
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    template <typename Allocator>
    memory_pool_allocated<Allocator>::memory_pool_allocated (
        const char* name, std::size_t blocks, std::size_t block_size_bytes,
        const attributes& _attributes, const allocator_type& allocator)
        : memory_pool{ name }
    {
#if defined(MICRO_OS_PLUS_TRACE_RTOS_MEMPOOL)
      trace::printf ("%s() @%p %s %d %d\n", __func__, this, this->name (),
                     blocks, block_size_bytes);
#endif
      if (_attributes.arena_address != nullptr)
        {
          // Do not use any allocator at all.
          internal_construct_ (blocks, block_size_bytes, _attributes, nullptr,
                               0);
        }
      else
        {
          allocator_ = &allocator;

          // If no user storage was provided via attributes,
          // allocate it dynamically via the allocator.
          allocated_pool_size_elements_
              = (compute_allocated_size_bytes<
                     typename allocator_type::value_type> (blocks,
                                                           block_size_bytes)
                 + sizeof (typename allocator_type::value_type) - 1)
                / sizeof (typename allocator_type::value_type);

          allocated_pool_arena_address_
              = const_cast<allocator_type&> (allocator).allocate (
                  allocated_pool_size_elements_);

          internal_construct_ (
              blocks, block_size_bytes, _attributes,
              allocated_pool_arena_address_,
              allocated_pool_size_elements_
                  * sizeof (typename allocator_type::value_type));
        }
    }

    /**
     * @details
     * This destructor shall destroy a memory pool object; the object
     * becomes, in effect, uninitialised. An implementation may cause
     * the destructor to set the object to an invalid value.
     *
     * It shall be safe to destroy an initialised memory pool object
     * upon which no threads are currently blocked. Attempting to
     * destroy a memory pool object upon which other threads are
     * currently blocked results in undefined behaviour.
     *
     * If the storage for the memory pool was dynamically allocated,
     * it is deallocated using the same allocator.
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    template <typename Allocator>
    memory_pool_allocated<Allocator>::~memory_pool_allocated ()
    {
#if defined(MICRO_OS_PLUS_TRACE_RTOS_MEMPOOL)
      trace::printf ("%s() @%p %s\n", __func__, this, name ());
#endif
      typedef typename std::allocator_traits<allocator_type>::pointer pointer;

      if (allocated_pool_arena_address_ != nullptr)
        {
          static_cast<allocator_type*> (const_cast<void*> (allocator_))
              ->deallocate (
                  static_cast<pointer> (allocated_pool_arena_address_),
                  allocated_pool_size_elements_);

          allocated_pool_arena_address_ = nullptr;
        }
    }

    // ========================================================================

    /**
     * @details
     * This constructor shall initialise a memory pool object
     * with attributes referenced by _attr_.
     * If the attributes specified by _attr_ are modified later,
     * the memory pool attributes shall not be affected.
     * Upon successful initialisation, the state of the memory pool
     * variable shall become initialised.
     *
     * Only the memory pool itself may be used for allocations.
     * It is not allowed to make copies of
     * condition variable objects.
     *
     * In cases where default memory pool attributes are
     * appropriate, the variable `memory_pool::initializer` can be used to
     * initialise condition variables.
     * The effect shall be equivalent to creating a memory pool object with
     * the simple constructor.
     *
     * If the attributes define a storage area (via `arena_address`
     * and `arena_size_bytes`), that storage is used, otherwise the
     * storage is dynamically allocated using the RTOS specific allocator
     * (`rtos::memory::allocator`).
     *
     * Implemented as a wrapper over the parent constructor, automatically
     * passing the message size.
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    template <typename T, typename Allocator>
    inline memory_pool_typed<T, Allocator>::memory_pool_typed (
        std::size_t blocks, const memory_pool::attributes& attributes,
        const allocator_type& allocator)
        : memory_pool_allocated<allocator_type> (blocks, sizeof (T),
                                                 attributes, allocator)
    {
      ;
    }

    /**
     * @details
     * This constructor shall initialise a named memory pool object
     * with attributes referenced by _attr_.
     * If the attributes specified by _attr_ are modified later,
     * the memory pool attributes shall not be affected.
     * Upon successful initialisation, the state of the memory pool
     * variable shall become initialised.
     *
     * Only the memory pool itself may be used for allocations.
     * It is not allowed to make copies of
     * condition variable objects.
     *
     * In cases where default memory pool attributes are
     * appropriate, the variable `memory_pool::initializer` can be used to
     * initialise condition variables.
     * The effect shall be equivalent to creating a memory pool object with
     * the simple constructor.
     *
     * If the attributes define a storage area (via `arena_address`
     * and `arena_size_bytes`), that storage is used, otherwise the
     * storage is dynamically allocated using the RTOS specific allocator
     * (`rtos::memory::allocator`).
     *
     * Implemented as a wrapper over the parent constructor, automatically
     * passing the message size.
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    template <typename T, typename Allocator>
    inline memory_pool_typed<T, Allocator>::memory_pool_typed (
        const char* name, std::size_t blocks,
        const memory_pool::attributes& attributes,
        const allocator_type& allocator)
        : memory_pool_allocated<allocator_type> (name, blocks, sizeof (T),
                                                 attributes, allocator)
    {
      ;
    }

    /**
     * @details
     * This destructor shall destroy a memory pool object; the object
     * becomes, in effect, uninitialised. An implementation may cause
     * the destructor to set the object to an invalid value.
     *
     * It shall be safe to destroy an initialised memory pool object
     * upon which no threads are currently blocked. Attempting to
     * destroy a memory pool object upon which other threads are
     * currently blocked results in undefined behaviour.
     *
     * If the storage for the memory pool was dynamically allocated,
     * it is deallocated using the same allocator.
     *
     * Implemented as a wrapper over the parent destructor.
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    template <typename T, typename Allocator>
    memory_pool_typed<T, Allocator>::~memory_pool_typed ()
    {
      ;
    }

    /**
     * @details
     * Wrapper over the parent method, automatically
     * passing the cast.
     *
     * @see memory_pool::alloc().
     */
    template <typename T, typename Allocator>
    inline typename memory_pool_typed<T, Allocator>::value_type*
    memory_pool_typed<T, Allocator>::alloc (void)
    {
      return static_cast<value_type*> (
          memory_pool_allocated<allocator_type>::alloc ());
    }

    /**
     * @details
     * Wrapper over the parent method, automatically
     * passing the cast.
     *
     * @see memory_pool::try_alloc().
     */
    template <typename T, typename Allocator>
    inline typename memory_pool_typed<T, Allocator>::value_type*
    memory_pool_typed<T, Allocator>::try_alloc (void)
    {
      return static_cast<value_type*> (
          memory_pool_allocated<allocator_type>::try_alloc ());
    }

    /**
     * @details
     * Wrapper over the parent method, automatically
     * passing the cast.
     *
     * @see memory_pool::timed_alloc().
     */
    template <typename T, typename Allocator>
    inline typename memory_pool_typed<T, Allocator>::value_type*
    memory_pool_typed<T, Allocator>::timed_alloc (clock::duration_t timeout)
    {
      return static_cast<value_type*> (
          memory_pool_allocated<allocator_type>::timed_alloc (timeout));
    }

    /**
     * @details
     * Wrapper over the parent method, automatically
     * passing the cast.
     *
     * @see memory_pool::free().
     */
    template <typename T, typename Allocator>
    inline result_t
    memory_pool_typed<T, Allocator>::free (value_type* block)
    {
      return memory_pool_allocated<allocator_type>::free (block);
    }

    // ========================================================================

    /**
     * @details
     * This constructor shall initialise a memory pool object
     * with attributes referenced by _attr_.
     * If the attributes specified by _attr_ are modified later,
     * the memory pool attributes shall not be affected.
     * Upon successful initialisation, the state of the memory pool
     * variable shall become initialised.
     *
     * Only the memory pool itself may be used for allocations.
     * It is not allowed to make copies of
     * condition variable objects.
     *
     * In cases where default memory pool attributes are
     * appropriate, the variable `memory_pool::initializer` can be used to
     * initialise condition variables.
     * The effect shall be equivalent to creating a memory pool object with
     * the simple constructor.
     *
     * The storage shall be statically allocated inside the
     * memory pool object instance.
     *
     * Implemented as a wrapper over the parent constructor, automatically
     * passing the message size and the storage details.
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    template <typename T, std::size_t N>
    memory_pool_inclusive<T, N>::memory_pool_inclusive (
        const attributes& _attributes)
    {
      internal_construct_ (blocks, sizeof (T), _attributes, &arena_,
                           sizeof (arena_));
    }

    /**
     * @details
     * This constructor shall initialise a named memory pool object
     * with attributes referenced by _attr_.
     * If the attributes specified by _attr_ are modified later,
     * the memory pool attributes shall not be affected.
     * Upon successful initialisation, the state of the memory pool
     * variable shall become initialised.
     *
     * Only the memory pool itself may be used for allocations.
     * It is not allowed to make copies of
     * condition variable objects.
     *
     * In cases where default memory pool attributes are
     * appropriate, the variable `memory_pool::initializer` can be used to
     * initialise condition variables.
     * The effect shall be equivalent to creating a memory pool object with
     * the simple constructor.
     *
     * The storage shall be statically allocated inside the
     * memory pool object instance.
     *
     * Implemented as a wrapper over the parent constructor, automatically
     * passing the message size and the storage details.
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    template <typename T, std::size_t N>
    memory_pool_inclusive<T, N>::memory_pool_inclusive (
        const char* name, const attributes& _attributes)
        : memory_pool{ name }
    {
      internal_construct_ (blocks, sizeof (T), _attributes, &arena_,
                           sizeof (arena_));
    }

    /**
     * @details
     * This destructor shall destroy the memory pool object; the object
     * becomes, in effect, uninitialised. An implementation may cause
     * the destructor to set the object to an invalid value.
     *
     * It shall be safe to destroy an initialised memory pool object
     * upon which no threads are currently blocked. Attempting to
     * destroy a memory pool object upon which other threads are
     * currently blocked results in undefined behaviour.
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    template <typename T, std::size_t N>
    memory_pool_inclusive<T, N>::~memory_pool_inclusive ()
    {
      ;
    }

    /**
     * @details
     * Wrapper over the parent method, automatically
     * passing the cast.
     *
     * @see memory_pool::alloc().
     */
    template <typename T, std::size_t N>
    inline typename memory_pool_inclusive<T, N>::value_type*
    memory_pool_inclusive<T, N>::alloc (void)
    {
      return static_cast<value_type*> (memory_pool::alloc ());
    }

    /**
     * @details
     * Wrapper over the parent method, automatically
     * passing the cast.
     *
     * @see memory_pool::try_alloc().
     */
    template <typename T, std::size_t N>
    inline typename memory_pool_inclusive<T, N>::value_type*
    memory_pool_inclusive<T, N>::try_alloc (void)
    {
      return static_cast<value_type*> (memory_pool::try_alloc ());
    }

    /**
     * @details
     * Wrapper over the parent method, automatically
     * passing the cast.
     *
     * @see memory_pool::timed_alloc().
     */
    template <typename T, std::size_t N>
    inline typename memory_pool_inclusive<T, N>::value_type*
    memory_pool_inclusive<T, N>::timed_alloc (clock::duration_t timeout)
    {
      return static_cast<value_type*> (memory_pool::timed_alloc (timeout));
    }

    /**
     * @details
     * Wrapper over the parent method, automatically
     * passing the cast.
     *
     * @see memory_pool::free().
     */
    template <typename T, std::size_t N>
    inline result_t
    memory_pool_inclusive<T, N>::free (value_type* block)
    {
      return memory_pool::free (block);
    }

  } // namespace rtos
} // namespace micro_os_plus

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------

#endif // __cplusplus

// ----------------------------------------------------------------------------

#endif // MICRO_OS_PLUS_RTOS_MEMORY_POOL_H_

// ----------------------------------------------------------------------------
