const FinancialSupp = () => {
  return (
    <div className="w-full mt-12 md:px-20 sm:px-12 px-6">
      <div className="site-card max-w-3xl mx-auto p-6">
        <h1 className="text-3xl font-bold mb-4 border-b-4 border-indigo-600 dark:border-indigo-300 inline-block pb-1 w-full text-center text-zinc-900 dark:text-slate-50">
          Financial Support
        </h1>

        <p className="mt-6 text-lg text-zinc-700 dark:text-slate-200">
          The ICNARI offers financial support to eligible students and faculty members from non-centrally funded technical institutes (CFTIs) in India, based on the recommendation of our experts.
        </p>

        <h2 className="mt-6 text-xl font-semibold text-indigo-700 dark:text-indigo-200">Eligibility Criteria</h2>
        <ul className="list-disc list-inside text-base mb-4 text-left inline-block text-zinc-700 dark:text-slate-200">
          <li><span className="font-semibold text-zinc-900 dark:text-slate-50">Students</span> who are authors of papers submitted to ICNARI.</li>
          <li><span className="font-semibold text-zinc-900 dark:text-slate-50">Students</span> enrolled in Ph.D., M.S. (by research), M.Tech, M.E., B.Tech, or B.E. programs in Indian institutions.</li>
          <li><span className="font-semibold text-zinc-900 dark:text-slate-50">Applicants</span> must remain students until at least December 31.</li>
          <li><span className="font-semibold text-zinc-900 dark:text-slate-50">Faculty members</span> from non-CFTIs working in systems and control.</li>
        </ul>

        <h2 className="mt-6 text-xl font-semibold text-indigo-700 dark:text-indigo-200">Support Provided</h2>
        <ul className="list-disc list-inside text-base mb-4 text-left inline-block text-zinc-700 dark:text-slate-200">
          <li>Registration fees</li>
          <li>Accommodation fees</li>
          <li>Partial travel support (if applicable)</li>
        </ul>

        <h2 className="mt-6 text-xl font-semibold text-indigo-700 dark:text-indigo-200">Application Process</h2>
        <ul className="list-disc list-inside text-base mb-4 text-left inline-block text-zinc-700 dark:text-slate-200">
          <li>Submit online applications through the form (link forthcoming).</li>
          <li>Deadline for application (will be announced).</li>
        </ul>

        <h2 className="mt-6 text-xl font-semibold text-indigo-700 dark:text-indigo-200">Reimbursement Process</h2>
        <ul className="list-disc list-inside text-base mb-4 text-left inline-block text-zinc-700 dark:text-slate-200">
          <li>Selected applicants must register for ICNARI.</li>
          <li>Reimbursement for registration, accommodation, and travel charges after conference participation.</li>
          <li>Full attendance during the conference is mandatory.</li>
        </ul>

        <h2 className="mt-6 text-xl font-semibold text-indigo-700 dark:text-indigo-200">Notification</h2>
        <p className="text-base mb-4 text-zinc-700 dark:text-slate-200">
          Selected applicants will be informed via email. Ensure accurate email address entry.
        </p>
      </div>
    </div>
  )
}

export default FinancialSupp
