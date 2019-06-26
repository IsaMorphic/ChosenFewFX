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