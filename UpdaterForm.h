#pragma once
#include "Updater.h"

namespace EvaUpdate {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Net;
	using namespace System::Threading;

	/// <summary>
	/// Summary for UpdaterForm
	/// </summary>
	public ref class UpdaterForm : public System::Windows::Forms::Form
	{
	public:
		String^ server;
		UpdaterForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			bool isAdmin;
			try
			{
				System::Security::Principal::WindowsIdentity^ user = System::Security::Principal::WindowsIdentity::GetCurrent();
				System::Security::Principal::WindowsPrincipal^ principal = gcnew System::Security::Principal::WindowsPrincipal(user);
				isAdmin = principal->IsInRole(System::Security::Principal::WindowsBuiltInRole::Administrator);
 			}
			catch (UnauthorizedAccessException^ ex)
			{
				isAdmin = false;
			}
			catch (Exception^ ex)
			{
				isAdmin = false;
			}
			if (!isAdmin)
			{
				MessageBox::Show("Você precisa entrar como 'Administrador'", "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
				exit(EXIT_FAILURE);
			}

			Thread^ connection;

			gcnew Updater;
			connection = gcnew Thread(gcnew ParameterizedThreadStart(&Updater::connectServerUpdate));
			connection->IsBackground = true;
			connection->Start(this);
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~UpdaterForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::PictureBox^  gif_loading;
	private: System::Windows::Forms::TextBox^  tb_principal;


	protected:

	protected:
	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(UpdaterForm::typeid));
			this->gif_loading = (gcnew System::Windows::Forms::PictureBox());
			this->tb_principal = (gcnew System::Windows::Forms::TextBox());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->gif_loading))->BeginInit();
			this->SuspendLayout();
			// 
			// gif_loading
			// 
			this->gif_loading->BackColor = System::Drawing::SystemColors::InactiveCaption;
			this->gif_loading->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"gif_loading.Image")));
			this->gif_loading->Location = System::Drawing::Point(107, 12);
			this->gif_loading->Name = L"gif_loading";
			this->gif_loading->Size = System::Drawing::Size(141, 125);
			this->gif_loading->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->gif_loading->TabIndex = 0;
			this->gif_loading->TabStop = false;
			// 
			// tb_principal
			// 
			this->tb_principal->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(229)), static_cast<System::Int32>(static_cast<System::Byte>(239)),
				static_cast<System::Int32>(static_cast<System::Byte>(241)));
			this->tb_principal->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->tb_principal->Cursor = System::Windows::Forms::Cursors::Arrow;
			this->tb_principal->Enabled = false;
			this->tb_principal->Font = (gcnew System::Drawing::Font(L"Calibri", 14.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->tb_principal->HideSelection = false;
			this->tb_principal->Location = System::Drawing::Point(39, 158);
			this->tb_principal->Name = L"tb_principal";
			this->tb_principal->ReadOnly = true;
			this->tb_principal->Size = System::Drawing::Size(274, 24);
			this->tb_principal->TabIndex = 1;
			this->tb_principal->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->tb_principal->WordWrap = false;
			// 
			// UpdaterForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(229)), static_cast<System::Int32>(static_cast<System::Byte>(239)),
				static_cast<System::Int32>(static_cast<System::Byte>(241)));
			this->ClientSize = System::Drawing::Size(351, 217);
			this->Controls->Add(this->tb_principal);
			this->Controls->Add(this->gif_loading);
			this->Cursor = System::Windows::Forms::Cursors::IBeam;
			this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
			this->Name = L"UpdaterForm";
			this->Text = L"Updater";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->gif_loading))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	};
}
