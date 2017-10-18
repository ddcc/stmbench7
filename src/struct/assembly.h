#ifndef SB7_ASSEMBLY_H_
#define SB7_ASSEMBLY_H_

#include "../tm/tm_ptr.h"

#include "../string.h"
#include "../containers.h"
#include "design_obj.h"
#include "module.h"

namespace sb7 {

	class ComplexAssembly;

	// Use this small hack in order to discover what type of object
	// we are dealing with in runtime.
	enum assembly_type {
		assembly_type_complex,
		assembly_type_base
	};

	class Assembly : public DesignObj {
		public:
			Assembly(int id, string type, int bd, sh_ptr<Module> module,
					sh_ptr<ComplexAssembly> superAssembly)
				: DesignObj(id, type, bd), m_superAssembly(superAssembly),
					m_module(module) {
			}

			virtual ~Assembly() { }

			sh_ptr<ComplexAssembly> getSuperAssembly() const {
				return m_superAssembly;
			}

			sh_ptr<Module> getModule() const {
				return m_module;
			}

			virtual enum assembly_type getType() const = 0;

		protected:
			sh_ptr<ComplexAssembly> m_superAssembly;
			sh_ptr<Module> m_module;
	};

	class ComplexAssembly : public Assembly {
		public:
			typedef Set<sh_ptr<Assembly> > assembly_set;
			typedef sh_ptr<assembly_set> shared_assembly_set;

		public:
			ComplexAssembly(int id, string type, int bd,
						sh_ptr<Module> module,
						sh_ptr<ComplexAssembly> superAssembly)
					: Assembly(id, type, bd, module, superAssembly),
					m_subAssemblies(shared_assembly_set(new assembly_set())) {
			}

			ComplexAssembly(const ComplexAssembly &ca)
					: Assembly(ca.m_id, ca.m_type, ca.m_buildDate, ca.m_module,
					ca.m_superAssembly), m_subAssemblies(ca.m_subAssemblies),
					m_level(ca.m_level) {
			}

			virtual ~ComplexAssembly() {
			}

			virtual ComplexAssembly* clone() const {
				return new ComplexAssembly(*this);
			}

			bool addSubAssembly(sh_ptr<Assembly> assembly) const;

			bool removeSubAssembly(sh_ptr<Assembly> assembly) const;

			shared_assembly_set getSubAssemblies() const {
				return m_subAssemblies;
			}

			short getLevel() const {
				return m_level;
			}

			bool areChildrenBaseAssemblies() const {
				return m_level == 2;
			}

			virtual enum assembly_type getType() const {
				return assembly_type_complex;
			}

			void freeMemory() {
				tx_delete(m_subAssemblies);
			}

			// This should be called after constructor. Don't call it inside it.
			void setLevel();

		protected:
			shared_assembly_set m_subAssemblies;
    		short m_level;
	};

	class CompositePart;

	class BaseAssembly : public Assembly {
		protected:
			typedef Bag<sh_ptr<CompositePart> > composite_part_bag;
			typedef sh_ptr<composite_part_bag> shared_composite_part_bag;

		public:
		    BaseAssembly(int id, string type, int buildDate,
			    		sh_ptr<Module> module,
		    			sh_ptr<ComplexAssembly> superAssembly)
					: Assembly(id, type, buildDate, module, superAssembly),
					m_components(new composite_part_bag()) {
			}

			BaseAssembly(const BaseAssembly &ba)
					: Assembly(ba.m_id, ba.m_type, ba.m_buildDate, ba.m_module,
					ba.m_superAssembly), m_components(ba.m_components) {
			}

			virtual ~BaseAssembly() {
			}

			virtual BaseAssembly* clone() const {
				return new BaseAssembly(*this);
			}

			void addComponent(sh_ptr<CompositePart> cpart) const;

			bool removeComponent(sh_ptr<CompositePart> cpart) const;

			shared_composite_part_bag getComponents() const {
				return m_components;
			}

			virtual enum assembly_type getType() const {
				return assembly_type_base;
			}

			void freeMemory() {
				tx_delete(m_components);
			}

		protected:
		    shared_composite_part_bag m_components;
	};
}

#endif /*SB7_ASSEMBLY_H_*/
