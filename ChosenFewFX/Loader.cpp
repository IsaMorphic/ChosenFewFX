/*
 *  Copyright 2019 Chosen Few Software
 *  This file is part of Chosen Few FX.
 *
 *  Chosen Few FX is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Chosen Few FX is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Chosen Few FX.  If not, see <https://www.gnu.org/licenses/>.
 */
class Loader {
	friend class constructor;

	struct constructor {
		constructor() {
			System::AppDomain^ currentDomain = System::AppDomain::CurrentDomain;
			currentDomain->AssemblyResolve += gcnew System::ResolveEventHandler(LoadFromSameFolder);
		}
	};

	static constructor cons;

	static System::Reflection::Assembly^ LoadFromSameFolder(System::Object^ sender, System::ResolveEventArgs^ args)
	{
		System::String^ folderPath = System::IO::Directory::GetParent(System::IO::Path::GetDirectoryName(System::Reflection::Assembly::GetExecutingAssembly()->Location))->FullName;
		System::String^ assemblyPath = System::IO::Path::Combine(folderPath, (gcnew System::Reflection::AssemblyName(args->Name))->Name + ".dll");
		if (!System::IO::File::Exists(assemblyPath)) return nullptr;
		System::Reflection::Assembly^ assembly = System::Reflection::Assembly::LoadFrom(assemblyPath);
		return assembly;
	}
};

// C++ needs to define static members externally.
Loader::constructor Loader::cons;